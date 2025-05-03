#include "builder.hpp"

#include <utility>

#pragma region constructors

interpreter::builder::builder(
    std::string const &settings_file_path,
    std::string const &operations_alphabet):
    _stream(settings_file_path),
    _operations_map(operations_alphabet)
{
}

interpreter::builder::builder(
    std::string const & settings_file_path):
    builder(settings_file_path, operations::_default_operations_alphabet)
{
}

#pragma endregion

#pragma region setters

interpreter::builder & interpreter::builder::append_function(
    std::string const &original_name,
    std::function<int(std::vector<int>)> function)
{
    _operations_map.upsert(original_name, std::move(function));
    return *this;
}

interpreter::builder & interpreter::builder::append_function_map(
    trie<std::function<int(std::vector<int>)>> const & functions_map)
{
    for (auto const & it : functions_map)
    {
        _operations_map.upsert(
            it->key,
            std::move(it->value));
    }
    return *this;
}

interpreter::builder & interpreter::builder::set_base_assign(size_t const & base)
{
    _base_assign = base;
    return *this;
}

interpreter::builder & interpreter::builder::set_base_input(size_t const & base)
{
    _base_input = base;
    return *this;
}

interpreter::builder & interpreter::builder::set_base_output(size_t const & base)
{
    _base_output = base;
    return *this;
}

interpreter::builder & interpreter::builder::set_comments_enclosure_max_level(size_t const & max_level)
{
    _comments_enclosure_max_level = max_level;
    return *this;
}

interpreter::builder & interpreter::builder::set_debug_mode(bool const & is_debug_mode_enabled)
{
    _is_debug_mode_enabled = is_debug_mode_enabled;
    return *this;
}

interpreter::builder & interpreter::builder::set_variables_alphabet(std::string const & alphabet)
{
    _variables_alphabet = alphabet;
    return *this;
}

#pragma endregion

#pragma region methods

interpreter * interpreter::builder::build()
{
    //parse operations_map, lvalues_position, arguments_position
    //make all arguments as reference

    lexer lexer( &_stream,
        _comments_enclosure_max_level,
        R"([\r\n\t ]+)",
        R"([^\r\n\t ])");

    std::string edit_operations_name;

    for (auto lexeme = lexer.begin_string_only();
        lexeme != lexer.end_string_only();
        ++lexeme)
    {
        auto lexeme_str = *lexeme;
        std::cout << lexeme_str << " ";


        if (lexeme_str=="left=")
        {
            _lvalues_position = interpreter::left;
        }
        else if (lexeme_str=="right=")
        {
            _lvalues_position = interpreter::right;
        }
        else if (lexeme_str=="op()")
        {
            _arguments_position = interpreter::after_operation;
        }
        else if (lexeme_str=="()op")
        {
            _arguments_position = interpreter::before_operation;
        }
        else if (lexeme_str=="(op)")
        {
            _arguments_position = interpreter::around_operation;
        }
        else
        {
            if (edit_operations_name.empty())
            {
                edit_operations_name = lexeme_str;
            }
            else
            {
                auto operation_func = _operations_map.obtain(edit_operations_name);
                _operations_map.dispose(edit_operations_name);
                _operations_map.upsert(lexeme_str, std::move(operation_func));
                edit_operations_name.clear();
            }
        }

    }

    if (!edit_operations_name.empty())
    {
        throw std::invalid_argument("The provided settings file contains invalid syntax.");
    }

    return new interpreter(
        _operations_map,
        _lvalues_position,
        _arguments_position,
        _comments_enclosure_max_level,
        _base_assign,
        _base_input,
        _base_output,
        _is_debug_mode_enabled,
        _variables_alphabet
        );
}

#pragma endregion