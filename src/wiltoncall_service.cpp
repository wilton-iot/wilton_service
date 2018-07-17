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

support::buffer service_get_pid(sl::io::span<const char>) {
    int pid = 0;
    char* err = wilton_service_get_pid(std::addressof(pid));
    if (nullptr != err) support::throw_wilton_error(err, TRACEMSG(err));
    return support::make_string_buffer(sl::support::to_string(pid));
}

support::buffer service_get_process_memory_size_bytes(sl::io::span<const char>) {
    int memory = 0;
    char* err = wilton_service_get_process_memory_size_bytes(std::addressof(memory));
    if (nullptr != err) support::throw_wilton_error(err, TRACEMSG(err));
    return support::make_string_buffer(sl::support::to_string(memory));
}

support::buffer service_get_threads_count(sl::io::span<const char>) {
    int count = 0;
    char* err = wilton_service_get_threads_count(std::addressof(count));
    if (nullptr != err) support::throw_wilton_error(err, TRACEMSG(err));
    return support::make_string_buffer(sl::support::to_string(count));
}

support::buffer service_increase_threads_count(sl::io::span<const char>) {
    char* err = wilton_service_increase_threads_count();
    if (nullptr != err) support::throw_wilton_error(err, TRACEMSG(err));
    return support::make_null_buffer();
}

support::buffer service_decrease_threads_count(sl::io::span<const char>) {
    wilton_service_decrease_threads_count();
    return support::make_null_buffer();
}

support::buffer service_get_call_stack(sl::io::span<const char>) {
    char* out = nullptr;
    int out_len = 0;
    char* err = wilton_service_get_call_stack(std::addressof(out), std::addressof(out_len));
    if (nullptr != err) support::throw_wilton_error(err, TRACEMSG(err));
    return support::make_string_buffer(std::string(out, out_len));
}

support::buffer service_get_all_calls(sl::io::span<const char>) {
    char* out = nullptr;
    int out_len = 0;
    char* error = wilton_service_get_all_calls(std::addressof(out), std::addressof(out_len));
    if (error) {
        return support::make_string_buffer(std::string(error));
    }
    return support::make_string_buffer(std::string(out, out_len));
}

support::buffer service_is_trace_info_gather_enabled(sl::io::span<const char>) {
    int is_enabled = 0;
    char* err = wilton_service_is_trace_info_gather_enabled(std::addressof(is_enabled));
    if (nullptr != err) support::throw_wilton_error(err, TRACEMSG(err));
    return support::make_string_buffer(sl::support::to_string(is_enabled));
}

support::buffer service_enable_trace_info_gather(sl::io::span<const char>) {
    char* err = wilton_service_enable_trace_info_gather();
    if (nullptr != err) support::throw_wilton_error(err, TRACEMSG(err));
    return support::make_null_buffer();
}

support::buffer service_disable_trace_info_gather(sl::io::span<const char>) {
    char* err = wilton_service_disable_trace_info_gather();
    if (nullptr != err) support::throw_wilton_error(err, TRACEMSG(err));
    return support::make_null_buffer();
}

} // namespace
}

extern "C" char* wilton_module_init() {
    try {
        // load config to chec is trace enabled
        char* config = nullptr;
        int config_len = 0;
        auto err_conf = wilton_config(std::addressof(config), std::addressof(config_len));
        if (nullptr != err_conf) wilton::support::throw_wilton_error(err_conf, TRACEMSG(err_conf));
        auto deferred = sl::support::defer([config] () STATICLIB_NOEXCEPT {
            wilton_free(config);
        }); // execute lambda on destruction

        // enable trace info gathering if flag enabled
        auto cf = sl::json::load({const_cast<const char*> (config), config_len});
        if (cf["traceEnabled"].as_bool()) {
            wilton_service_enable_trace_info_gather();
        }

        // register functions
        wilton::support::register_wiltoncall("service_get_pid", wilton::service::service_get_pid);
        wilton::support::register_wiltoncall("service_get_process_memory_size_bytes", wilton::service::service_get_process_memory_size_bytes);
        wilton::support::register_wiltoncall("service_get_threads_count", wilton::service::service_get_threads_count);
        wilton::support::register_wiltoncall("service_increase_threads_count", wilton::service::service_increase_threads_count);
        wilton::support::register_wiltoncall("service_decrease_threads_count", wilton::service::service_decrease_threads_count);
        wilton::support::register_wiltoncall("service_get_call_stack", wilton::service::service_get_call_stack);
        wilton::support::register_wiltoncall("service_get_all_calls", wilton::service::service_get_all_calls);
        wilton::support::register_wiltoncall("service_enable_trace_info_gather", wilton::service::service_enable_trace_info_gather);
        wilton::support::register_wiltoncall("service_disable_trace_info_gather", wilton::service::service_disable_trace_info_gather);
        wilton::support::register_wiltoncall("service_is_trace_info_gather_enabled", wilton::service::service_is_trace_info_gather_enabled);

        return nullptr;
    } catch (const std::exception& e) {
        return wilton::support::alloc_copy(TRACEMSG(e.what() + "\nException raised"));
    }
}
