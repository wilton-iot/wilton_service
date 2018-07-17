/*
 * Copyright 2018, mike at myasnikov.mike@gmail.com
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

#ifndef WILTON_SERVICE_TRACE_INFO_HPP
#define WILTON_SERVICE_TRACE_INFO_HPP

#include <string>

#include "staticlib/pimpl.hpp"
#include "wilton/support/exception.hpp"

namespace wilton {
namespace service {

class trace_info : public sl::pimpl::object {
protected:
    /**
     * implementation class
     */
    class impl;
public:
    /**
     * PIMPL-specific constructor
     * 
     * @param pimpl impl object
     */
    PIMPL_CONSTRUCTOR(trace_info)

    static int32_t service_start_call(std::string call);
    
    static void service_finish_call(std::string result, int32_t id);

    static std::string service_get_call_stack();

    static std::string service_get_all_calls();

    static bool is_trace_info_gather_enabled();

    static void enable_trace_info_gather();

    static void disable_trace_info_gather();
};

} // namespace
}

#endif /* WILTON_SERVICE_TRACE_INFO_HPP */
