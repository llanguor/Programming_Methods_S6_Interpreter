#pragma once
#include "trie.hpp"
#include <functional>

class interpreter
{

private:

    enum lvalues_position_types
    {
        left,
        right
    };

    enum arguments_position_types
    {
        after_operation,
        around_operation,
        before_operation,
    };

public:

    static std::string const _default_variables_alphabet;

private:

    trie<int> _variables;

    trie<std::function<int(std::vector<int>)>> const _operations;
    lvalues_position_types const _lvalues_position;
    arguments_position_types const _arguments_position;
    bool const _is_debug_mode_enabled;

    size_t const _base_assign;
    size_t const _base_input;
    size_t const _base_output;



public:

    interpreter(
        trie<std::function<int(std::vector<int>)>> const &_operations,
        lvalues_position_types const & _lvalues_position,
        arguments_position_types const & _arguments_position,
        size_t const & _base_assign,
        size_t const & _base_input,
        size_t const & _base_output,
        bool const & _is_debug_mode_enabled
        );

public:

    class builder;
};