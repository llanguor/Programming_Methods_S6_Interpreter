#include "interpreter.hpp"

#pragma region static_fields

std::string const interpreter::_default_variables_alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";

#pragma endregion

#pragma region constructors

interpreter::interpreter(

    trie<std::function<int(std::vector<int>)>> const &operations,
    lvalues_position_types const &lvalues_position,
    arguments_position_types const &arguments_position,
    size_t const & comments_enclosure_max_level,
    size_t const &base_assign,
    size_t const &base_input,
    size_t const &base_output,
    bool const &is_debug_mode_enabled,
    std::string const & variables_alphabet):

    _operations(operations),
    _lvalues_position(lvalues_position),
    _arguments_position(arguments_position),
    _comments_enclosure_max_level(comments_enclosure_max_level),
    _base_assign(base_assign),
    _base_input(base_input),
    _base_output(base_output),
    _is_debug_mode_enabled(is_debug_mode_enabled),
    _variables(variables_alphabet)
{
}

#pragma endregion

#pragma region setters

interpreter & interpreter::set_base_assign(size_t const &base)
{
    _base_assign = base;
    return *this;
}

interpreter & interpreter::set_base_input(size_t const &base)
{
    _base_input = base;
    return *this;
}

interpreter & interpreter::set_base_output(size_t const &base)
{
    _base_output = base;
    return *this;
}

interpreter & interpreter::set_debug_mode(bool const &is_debug_mode_enabled)
{
    _is_debug_mode_enabled = is_debug_mode_enabled;
    return *this;
}

#pragma endregion

#pragma region methods

void interpreter::run(std::string const &program_file_path, bool const &is_debug_mode_enabled)
{
    run(program_file_path, is_debug_mode_enabled, _base_assign, _base_input, _base_output);
}

void interpreter::run(std::string const &program_file_path)
{
    run(program_file_path, _is_debug_mode_enabled, _base_assign, _base_input, _base_output);
}

void interpreter::run(
    std::string const &program_file_path,
    bool const &is_debug_mode_enabled,
    size_t const &base_assign,
    size_t const &base_input,
    size_t const &base_output)
{
    // lexer _lexer;
}

#pragma endregion