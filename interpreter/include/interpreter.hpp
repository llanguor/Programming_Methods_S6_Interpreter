#pragma once
#include "trie.hpp"
#include <functional>

class interpreter
{


    //возможно потребуется создать lexer. Тогда хранить его в ПОЛЕ
    //передавать в этот конструктор параметры для создания
    //создавать должен КОНСТРУКТОР

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
        trie<std::function<int(std::vector<int>)>> const &operations,
        lvalues_position_types const & lvalues_position,
        arguments_position_types const & arguments_position,
        size_t const & base_assign,
        size_t const & base_input,
        size_t const & base_output,
        bool const & is_debug_mode_enabled,
        std::string const & variables_alphabet
        );

public:

    class builder;
};