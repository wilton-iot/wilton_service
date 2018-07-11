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

#include "trace_info.hpp"

#include "staticlib/json.hpp"
#include "staticlib/pimpl/forward_macros.hpp"
#include "wilton/support/exception.hpp"
#include "staticlib/utils/string_utils.hpp"

#include <list>
#include <memory>
#include <map>

namespace wilton {
namespace service {

class trace_node;
namespace {
using call_string = std::string;
std::string get_json_value_as_string(const sl::json::value* val){
    std::string result{};

    switch (val->json_type()) {
    case sl::json::type::string: {
        result = val->as_string();
        break;
    }
    case sl::json::type::integer: {
        result = sl::support::to_string(val->as_int64());
        break;
    }
    case sl::json::type::real: {
        result = sl::support::to_string(val->as_float());
        break;
    }
    case sl::json::type::boolean: {
        result = sl::support::to_string(val->as_bool());
        break;
    }
    case sl::json::type::array: {
        result += "[";
        for (auto& el : val->as_array()) {
            result += get_json_value_as_string(&el);
            result += ",";
        }
        result += "]";
        break;
    }
    case sl::json::type::object:{
        std::string dump = val->dumps();
        dump = sl::utils::replace_all(dump, "\n", "");
        dump = sl::utils::replace_all(dump, "    ", " ");
        result += dump;
        break;
    }
    default:
        break;
    }

    return result;
}
std::string parse_json(std::string json){
    std::string line{};
    sl::json::value val = sl::json::loads(json);
    for (auto& el : val.as_object()) {
        if ("args" == el.name()) {
            line += "(";
            for (auto& arg : el.as_array()) {
                line += get_json_value_as_string(&arg) + ", ";
            }
            if (el.as_array().size()) { // remove 2 last ', ' symbols
                line.pop_back();
                line.pop_back();
            }
            line += ")";
        } else {
            line += el.as_string() + "/";
        }
    }
    return line;
}
using trace_node_shared_ptr = std::shared_ptr<trace_node>;
using trace_node_ptr = trace_node*;
using id_type = int32_t;
}

class trace_node :public std::enable_shared_from_this<trace_node>
{
    std::string call;
    std::string result;
    std::list<trace_node_shared_ptr> childs;
    trace_node_ptr parent;

    // id allows index nodes and determine to which belongs recieved result data(result option)
    static id_type uniq_id;
    id_type id;

    std::string get_formatted_string(std::string tab, trace_node_ptr node) {
        std::string res{};
        std::string new_tab = tab + tab;
        res += parse_json(node->call);
        res += "\n";
        for (auto& child : node->childs) {
            res += tab;
            res += get_formatted_string(new_tab, child.get());
        }
        if (!result.empty()) {
            res += "\n";
            res += tab;
            res += " ^--> ";
            res += result;
        }
        return res;
    }

public:
    trace_node(std::string call, trace_node_ptr parent) : call(call), parent(parent), id(uniq_id++) {}
    trace_node(std::string call) : call(call), parent(nullptr), id(uniq_id++) {}
    id_type add_child(std::string call) {
        auto child = std::make_shared<trace_node>(call, this);
        childs.push_back(child);
        return child->get_id();
    }
    int get_childs_count() {
        return childs.size();
    }
    trace_node_ptr get_last_child(){
        return childs.back().get();
    }
    trace_node_ptr get_parent(){
        return parent;
    }
    std::string get_tree_view(){
        std::string res{};
        res = get_formatted_string("  ", childs.front().get());
        return res;
    }
    void set_result(std::string res){
        this->result = res;
    }
    id_type get_id(){
        return id;
    }
};

class trace_info::impl : public staticlib::pimpl::object::impl {
private:
    static std::list<call_string> call_stack;
    static std::string prepare_formatted_call_stack(){
        std::string line{};
        for (auto& call : call_stack){
            line += parse_json(call);
            line += "\n";
        }
        return line;
    }

    static trace_node tree;
    static trace_node_ptr node;
    // node pointers are valid until tree will be deleted
    static std::map<id_type, trace_node_ptr> indexed_nodes;
    static bool trace_info_gather_enabled;
public:
    static id_type service_start_call(std::string call){
        id_type id = 0;
        if (trace_info_gather_enabled) {
            call_stack.push_back(call);
            id = node->add_child(call);
            node = node->get_last_child();
            indexed_nodes[id] = node;
        }
        return id;
    }
    static void service_finish_call(std::string result, int id){
        if (trace_info_gather_enabled) {
            call_stack.pop_back();
            node = node->get_parent();
            indexed_nodes[id]->set_result(result);
        }
    }
    static std::string service_get_call_stack(){
        if (trace_info_gather_enabled) {
            return prepare_formatted_call_stack();
        }
        return std::string{};
    }
    static std::string service_get_all_calls(){
        if (trace_info_gather_enabled) {
            return tree.get_tree_view();
        }
        return std::string{};
    }
    static bool is_trace_info_gather_enabled(){
        return  trace_info_gather_enabled;
    }
    static void enable_trace_info_gather(){
        trace_info_gather_enabled = true;
    }
    static void disable_trace_info_gather(){
        trace_info_gather_enabled = false;
    }
};

// init static members
id_type trace_node::uniq_id{0};

trace_node trace_info::impl::tree{"{\"tree_root\": \"root\"}"};
trace_node_ptr trace_info::impl::node = &trace_info::impl::tree;
std::list<call_string> trace_info::impl::call_stack{};
std::map<id_type, trace_node_ptr> trace_info::impl::indexed_nodes;
bool trace_info::impl::trace_info_gather_enabled{false};

PIMPL_FORWARD_METHOD_STATIC(trace_info, id_type, service_start_call, (std::string), (), support::exception)
PIMPL_FORWARD_METHOD_STATIC(trace_info, void, service_finish_call, (std::string)(id_type), (), support::exception)
PIMPL_FORWARD_METHOD_STATIC(trace_info, std::string, service_get_call_stack, (), (), support::exception)
PIMPL_FORWARD_METHOD_STATIC(trace_info, std::string, service_get_all_calls, (), (), support::exception)
PIMPL_FORWARD_METHOD_STATIC(trace_info, bool, is_trace_info_gather_enabled, (), (), support::exception)
PIMPL_FORWARD_METHOD_STATIC(trace_info, void, enable_trace_info_gather, (), (), support::exception)
PIMPL_FORWARD_METHOD_STATIC(trace_info, void, disable_trace_info_gather, (), (), support::exception)

} // namespace
}
