#pragma once
#include "trie.hpp"
#include <functional>
#include "tokenizer.hpp"
#include "operations.hpp"

//todo: add comments everywhere. get used to writing. also use built-in method definitions

class interpreter
{
public:

    friend class debugger;

    template<typename interpreter_type> class builder;

public:

    static std::string const default_variables_alphabet;

    static struct system_variables
    {
    public:
        static const std::string base_input;
        static const std::string base_output;
        static const std::string base_assign;
    }  system_variables;

protected:

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

protected:

    //trie<std::string> _operands_ident; //for =();
    trie<std::function<int(std::vector<int>)>> _operations;
    lvalues_position_types const _lvalues_position;
    arguments_position_types const _arguments_position;
    size_t const _comments_enclosure_max_level;

protected:

    bool _is_debug_mode_enabled;
    size_t _base_assign;
    size_t _base_input;
    size_t _base_output;

protected:

    trie<int> _variables;
    std::string const _instructions_alphabet = R"([A-Za-z0-9!#$%&'()*+,-./:<=>?@[\]^_`{|}~])";

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

private:

    virtual int calculate_expression(
        std::string const & expression, int const & line_number) = 0;

public:

    virtual ~interpreter() = default;

};