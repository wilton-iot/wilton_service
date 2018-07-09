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
 * File:   wilton_service.cpp
 * Author: alex
 *
 * Created on July 8, 2018, 8:49 PM
 */

#include "wilton/wilton_service.h"


#include "wilton/support/alloc.hpp"
#include "threads_counter.hpp"
#include "process_info.hpp"

// process info
char* wilton_service_get_pid(int* pid_out) /* noexcept */ {
    if (nullptr == pid_out) return wilton::support::alloc_copy(TRACEMSG("Null 'pid_out' parameter specified"));
    try {
        *pid_out = wilton::service::process_info::service_get_pid();
        return nullptr;
    } catch (const std::exception& e) {
        return wilton::support::alloc_copy(TRACEMSG(e.what() + "\nException raised"));
    }
}
char* wilton_service_get_process_memory_size_bytes(int* memory_out) /* noexcept */ {
    if (nullptr == memory_out) return wilton::support::alloc_copy(TRACEMSG("Null 'memory_out' parameter specified"));
    try {
        *memory_out = wilton::service::process_info::service_get_process_memory_size_bytes();
        return nullptr;
    } catch (const std::exception& e) {
        return wilton::support::alloc_copy(TRACEMSG(e.what() + "\nException raised"));
    }
}

// threads count
char* wilton_service_get_threads_count(int* count_out) /* noexcept */ {
    if (nullptr == count_out) return wilton::support::alloc_copy(TRACEMSG("Null 'count_out' parameter specified"));
    try {
        *count_out = wilton::service::threads_counter::get_threads_count();
        return nullptr;
    } catch (const std::exception& e) {
        return wilton::support::alloc_copy(TRACEMSG(e.what() + "\nException raised"));
    }
}
char* wilton_service_increase_threads_count() /* noexcept */ {
    try {
        wilton::service::threads_counter::increase_threads_count();
        return nullptr;
    } catch (const std::exception& e) {
        return wilton::support::alloc_copy(TRACEMSG(e.what() + "\nException raised"));
    }
}
char* wilton_service_decrease_threads_count() /* noexcept */ {
    try {
        wilton::service::threads_counter::decrease_threads_count();
        return nullptr;
    } catch (const std::exception& e) {
        return wilton::support::alloc_copy(TRACEMSG(e.what() + "\nException raised"));
    }
}

