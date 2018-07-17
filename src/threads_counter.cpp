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

#include "threads_counter.hpp"

#include <atomic>

#include "staticlib/pimpl/forward_macros.hpp"
#include "wilton/support/exception.hpp"

namespace wilton {
namespace service {

class threads_counter::impl : public staticlib::pimpl::object::impl {
private:
    static std::atomic<uint32_t> threads_count;

public:
    static void increase_threads_count() {
        threads_count.fetch_add(1, std::memory_order_acq_rel);
    }

    static void decrease_threads_count() {
        threads_count.fetch_sub(1, std::memory_order_acq_rel);
    }

    static uint32_t get_threads_count() {
        return threads_count.load(std::memory_order_acquire);
    }
};

std::atomic<uint32_t> threads_counter::impl::threads_count{0};

PIMPL_FORWARD_METHOD_STATIC(threads_counter, void, increase_threads_count, (), (), support::exception)
PIMPL_FORWARD_METHOD_STATIC(threads_counter, void, decrease_threads_count, (), (), support::exception)
PIMPL_FORWARD_METHOD_STATIC(threads_counter, uint32_t,  get_threads_count, (), (), support::exception)

} // namespace
}
