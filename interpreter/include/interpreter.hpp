#pragma once
#include "trie.hpp"
#include <functional>

#include "lexer.hpp"


//возможно потребуется создать lexer. Тогда хранить его в ПОЛЕ
//передавать в этот конструктор параметры для создания
//создавать должен КОНСТРУКТОР


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

    //trie<std::string> _operands_ident; //for =();
    trie<std::function<int(std::vector<int>)>> const _operations;
    lvalues_position_types const _lvalues_position;
    arguments_position_types const _arguments_position;
    size_t const _comments_enclosure_max_level;

private:

    bool _is_debug_mode_enabled;
    size_t _base_assign;
    size_t _base_input;
    size_t _base_output;

private:

    trie<int> _variables;

public:

    interpreter(
        trie<std::function<int(std::vector<int>)>> const &operations,
        lvalues_position_types const & lvalues_position,
        arguments_position_types const & arguments_position,
        size_t const & comments_enclosure_max_level,
        size_t const & base_assign,
        size_t const & base_input,
        size_t const & base_output,
        bool const & is_debug_mode_enabled,
        std::string const & variables_alphabet
        );

public:

    //TODO: validate + in builder

    interpreter& set_base_assign(
           size_t const & base);

    interpreter& set_base_input(
        size_t const & base);

    interpreter& set_base_output(
        size_t const & base);

    interpreter& set_debug_mode(
        bool const & is_debug_mode_enabled);

public:

    void run(
        std::string const & program_file_path);

    void run(
        std::string const & program_file_path,
        bool const & is_debug_mode_enabled);

    void run(
    std::string const & program_file_path,
    bool const & is_debug_mode_enabled,
    size_t const & base_assign,
    size_t const & base_input,
    size_t const & base_output);

public:

    class builder;
};