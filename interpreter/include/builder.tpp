#pragma once
#include "builder.hpp"


#pragma region constructors

template<typename interpreter_type>
interpreter::builder<interpreter_type>::builder(
    std::string const &settings_file_path,
    std::string const &operations_alphabet):
    _stream(settings_file_path),
    _operations_map(operations_alphabet)
{
    if (!_stream.is_open())
    {
        throw std::runtime_error("Failed to open file: " + settings_file_path +
                            ". File might not exist or you do not have permission to read it.");
    }

    if (_stream.fail())
    {
        throw std::runtime_error("Stream operation failed: either the file could not be read or the data could not be extracted.");
    }
}

template<typename interpreter_type>
interpreter::builder<interpreter_type>::builder(
    std::string const & settings_file_path):
    builder(settings_file_path, operations::default_operations_alphabet)
{
}

#pragma endregion

#pragma region setters

template<typename interpreter_type>
interpreter::builder<interpreter_type> & interpreter::builder<interpreter_type>::append_function(
    std::string const &original_name,
    std::function<int(std::vector<int>)> function)
{
    _operations_map.upsert(original_name, std::move(function));
    return *this;
}

template<typename interpreter_type>
interpreter::builder<interpreter_type> & interpreter::builder<interpreter_type>::append_function_map(
    trie<std::function<int(std::vector<int>)>> const & functions_map)
{
    for (auto & it : functions_map)
    {
        _operations_map.upsert(
            it.key,
            std::move(it.value));
    }
    return *this;
}

template<typename interpreter_type>
interpreter::builder<interpreter_type> & interpreter::builder<interpreter_type>::set_base_assign(size_t const & base)
{
    if (_base_assign<2 || _base_assign>36)
    {
        throw std::invalid_argument("base must be from 2 to 36");
    }

    _base_assign = base;
    return *this;
}

template<typename interpreter_type>
interpreter::builder<interpreter_type> & interpreter::builder<interpreter_type>::set_base_input(size_t const & base)
{
    if (_base_assign<2 || _base_assign>36)
    {
        throw std::invalid_argument("base must be from 2 to 36");
    }

    _base_input = base;
    return *this;
}

template<typename interpreter_type>
interpreter::builder<interpreter_type> & interpreter::builder<interpreter_type>::set_base_output(size_t const & base)
{
    if (_base_assign<2 || _base_assign>36)
    {
        throw std::invalid_argument("base must be from 2 to 36");
    }

    _base_output = base;
    return *this;
}

template<typename interpreter_type>
interpreter::builder<interpreter_type> & interpreter::builder<interpreter_type>::set_comments_enclosure_max_level(size_t const & max_level)
{
    _comments_enclosure_max_level = max_level;
    return *this;
}

template<typename interpreter_type>
interpreter::builder<interpreter_type> & interpreter::builder<interpreter_type>::set_debug_mode(bool const & is_debug_mode_enabled)
{
    _is_debug_mode_enabled = is_debug_mode_enabled;
    return *this;
}

template<typename interpreter_type>
interpreter::builder<interpreter_type> & interpreter::builder<interpreter_type>::set_variables_alphabet(std::string const & alphabet)
{
    _variables_alphabet = alphabet;
    return *this;
}

#pragma endregion

#pragma region methods

template<typename interpreter_type>
interpreter_type * interpreter::builder<interpreter_type>::build()
{
    //parse operations_map, lvalues_position, arguments_position
    //make all arguments as reference

    tokenizer tokenizer( &_stream,
        _comments_enclosure_max_level,
        R"([\r\n\t ]+)",
        R"([^\r\n\t ])");

    std::string edit_operations_name;

    for (auto token = tokenizer.begin_string_only();
        token != tokenizer.end_string_only();
        ++token)
    {
        auto token_str = token->token;

        if (token_str=="left=")
        {
            _lvalues_position = interpreter::left;
        }
        else if (token_str=="right=")
        {
            _lvalues_position = interpreter::right;
        }
        else if (token_str=="op()")
        {
            _arguments_position = interpreter::after_operation;
        }
        else if (token_str=="()op")
        {
            _arguments_position = interpreter::before_operation;
        }
        else if (token_str=="(op)")
        {
            _arguments_position = interpreter::around_operation;
        }
        else
        {
            if (edit_operations_name.empty())
            {
                edit_operations_name = token_str;
            }
            else
            {
                auto operation_func = _operations_map.obtain(edit_operations_name);
                _operations_map.dispose(edit_operations_name);
                _operations_map.upsert(token_str, std::move(operation_func));
                edit_operations_name.clear();
            }
        }

    }

    if (!edit_operations_name.empty())
    {
        throw std::invalid_argument("The provided settings file contains invalid syntax.");
    }

    return new interpreter_type(
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