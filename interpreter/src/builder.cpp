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

interpreter::builder & interpreter::builder::set_base_assign(size_t base)
{
    _base_assign = base;
    return *this;
}

interpreter::builder & interpreter::builder::set_base_input(size_t base)
{
    _base_input = base;
    return *this;
}

interpreter::builder & interpreter::builder::set_base_output(size_t base)
{
    _base_output = base;
    return *this;
}

interpreter::builder & interpreter::builder::set_debug_mode(bool is_debug_mode_enabled)
{
    _is_debug_mode_enabled = is_debug_mode_enabled;
    return *this;
}

interpreter::builder & interpreter::builder::set_variables_alphabet(std::string const &alphabet)
{
    _variables_alphabet = alphabet;
    return *this;
}

#pragma endregion

#pragma region methods

int interpreter::builder::check_function(
    std::string const &original_name)
{
    return _operations_map.obtain(original_name)({2,3});;
}

interpreter const * interpreter::builder::build() const
{

    //parse operations_map, lvalues_position, arguments_position
    //make all arguments as reference

        /*
    for (auto l : _lexer)
    {
        if (std::holds_alternative<lexer::control_char_types>(l))
        {
            throw std::invalid_argument("the use of control codes is not supported in the context of a settings file");
        }

        std::cout << std::get<std::string>(l) <<" ";

    }
    */

    return new interpreter(
        _operations_map,
        _lvalues_position,
        _arguments_position,
        _base_assign,
        _base_input,
        _base_output,
        _is_debug_mode_enabled
        );
}

#pragma endregion