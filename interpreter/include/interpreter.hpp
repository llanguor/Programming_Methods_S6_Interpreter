#pragma once
#include "trie.hpp"
#include <functional>

class interpreter
{

public:

    static std::string const _default_variables_alphabet;

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

private:

    trie<int> _variables;
    size_t _base_assign = 10, _base_input=10, _base_output=10;
    trie<std::function<int(std::vector<int>)>> _functions_map;
    lvalues_position_types _lvalues_position = left;
    arguments_position_types _arguments_position = after_operation;



public:

    class builder;
};