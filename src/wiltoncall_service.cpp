/*
 * Copyright 2018, alex at staticlibs.net
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

/* 
 * File:   wiltoncall_service.cpp
 * Author: alex
 *
 * Created on July 8, 2018, 8:49 PM
 */


#include "staticlib/support.hpp"
#include "staticlib/json.hpp"
#include "staticlib/utils.hpp"

#include "wilton/wilton_service.h"

#include "wilton/support/buffer.hpp"
#include "wilton/support/exception.hpp"
#include "wilton/support/registrar.hpp"

namespace wilton {
namespace service {


support::buffer service_get_pid(sl::io::span<const char> ) {
    int pid = 0;
    wilton_service_get_pid(&pid);
	return support::make_string_buffer(sl::support::to_string(pid));
}
support::buffer service_get_process_memory_size_bytes(sl::io::span<const char> ) {
    int memory = 0;
    wilton_service_get_process_memory_size_bytes(&memory);
	return support::make_string_buffer(sl::support::to_string(memory));
}
support::buffer service_get_threads_count(sl::io::span<const char> ) {
	int count = 0;
    wilton_service_get_threads_count(&count);
    return support::make_string_buffer(sl::support::to_string(count));
}
support::buffer service_increase_threads_count(sl::io::span<const char> ) {
    wilton_service_increase_threads_count();
    return support::make_null_buffer();
}
support::buffer service_decrease_threads_count(sl::io::span<const char> ) {
    wilton_service_decrease_threads_count();
    return support::make_null_buffer();
}

} // namespace
}

extern "C" char* wilton_module_init() {
    try {
        wilton::support::register_wiltoncall("service_get_pid", wilton::service::service_get_pid);
        wilton::support::register_wiltoncall("service_get_process_memory_size_bytes", wilton::service::service_get_process_memory_size_bytes);
        wilton::support::register_wiltoncall("service_get_threads_count", wilton::service::service_get_threads_count);
        wilton::support::register_wiltoncall("service_increase_threads_count", wilton::service::service_increase_threads_count);
        wilton::support::register_wiltoncall("service_decrease_threads_count", wilton::service::service_decrease_threads_count);
        return nullptr;
    } catch (const std::exception& e) {
        return wilton::support::alloc_copy(TRACEMSG(e.what() + "\nException raised"));
    }
}
