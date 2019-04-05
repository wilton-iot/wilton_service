/*
 * Copyright 2018, mike at myasnikov.mike@gmail.com
 * Copyright 2018, alex at staticlibs.net
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "process_info.hpp"

#include "staticlib/config/os.hpp"

#ifdef STATICLIB_WINDOWS
#include "staticlib/support/windows.hpp"
#include <psapi.h>
#else // !STATICLIB_WINDOWS
#include <sys/types.h>
#include <unistd.h>
#endif // STATICLIB_WINDOWS

#include "staticlib/pimpl/forward_macros.hpp"
#include "wilton/support/exception.hpp"

namespace wilton {
namespace service {

class process_info::impl : public staticlib::pimpl::object::impl {
public:

    static int64_t service_get_pid() {
        int64_t pid = 0; // beacause getpid is int32_t, but GetCurrentProcessId returns DWORD which uint32_t
#ifdef STATICLIB_WINDOWS
        pid = GetCurrentProcessId();
#else // !STATICLIB_WINDOWS
        pid = ::getpid();
#endif // STATICLIB_WINDOWS
        return pid;
    }

    static int64_t service_get_process_memory_size_bytes() {
#ifdef STATICLIB_WINDOWS
        PROCESS_MEMORY_COUNTERS pmc;
        // THere may be different PSAPI_VERSION, but it always use GetProcessMemoryInfo function name
        GetProcessMemoryInfo(GetCurrentProcess(), std::addressof(pmc), sizeof(pmc));
        // https://docs.microsoft.com/en-us/windows/desktop/api/psapi/ns-psapi-_process_memory_counters
        // PagefileUsage - The Commit Charge value in bytes for this process. 
        // Commit Charge is the total amount of memory that the memory manager has committed for a running process.
        return pmc.PagefileUsage;
#else // !STATICLIB_WINDOWS
        return -1;
#endif // STATICLIB_WINDOWS
    }
};

PIMPL_FORWARD_METHOD_STATIC(process_info, int64_t, service_get_pid, (), (), support::exception)
PIMPL_FORWARD_METHOD_STATIC(process_info, int64_t, service_get_process_memory_size_bytes, (), (), support::exception)

} // namespace
}
