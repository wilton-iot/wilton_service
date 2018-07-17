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

#ifndef WILTON_SERVICE_PROCESS_INFO_HPP
#define WILTON_SERVICE_PROCESS_INFO_HPP

#include <string>

#include "staticlib/pimpl.hpp"
#include "wilton/support/exception.hpp"

namespace wilton {
namespace service {

class process_info : public sl::pimpl::object {
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
    PIMPL_CONSTRUCTOR(process_info)

    static int64_t service_get_pid();

    static int64_t service_get_process_memory_size_bytes();
};


} // namespace
}

#endif /* WILTON_SERVICE_PROCESS_INFO_HPP */
