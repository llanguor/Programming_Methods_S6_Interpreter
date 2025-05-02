#include "builder.hpp"

#include <utility>

interpreter::builder::builder(
    std::string const & settings_file_path,
    int enclosure_max_level,
    size_t base_assign,
    size_t base_input,
    size_t base_output,
    bool is_debug,
    std::string variables_alphabet,
    std::string const & functions_alphabet):

    _base_assign(base_assign),
    _base_input(base_input),
    _base_output(base_output),
    _is_debug(is_debug),
    _stream(settings_file_path),
    _lexer(&_stream, enclosure_max_level, "[a-zA-Z0-9_]", "[\r\n\t ]+"),
    _variables_alphabet(std::move(variables_alphabet)),
    _functions_map(functions_alphabet)
{
    for (auto l : _lexer)
    {
        if (std::holds_alternative<lexer::control_char_types>(l))
        {
            throw std::invalid_argument("the use of control codes is not supported in the context of a settings file");
        }

        std::cout << std::get<std::string>(l) <<" ";

    }
}



interpreter::builder & interpreter::builder::append_function(
    std::string const &original_name,
    std::function<int(std::vector<int>)> function)
{
    _functions_map.upsert(original_name, std::move(function));
    return *this;
}

interpreter::builder & interpreter::builder::append_function_map(
    trie<std::function<int(std::vector<int>)>> & functions_map)
{
    for (auto const & it : functions_map)
    {
        _functions_map.upsert(
            it->key,
            std::move(it->value));
    }
    return *this;
}

int interpreter::builder::check_function(
    std::string const &original_name)
{
    return _functions_map.obtain(original_name)({2,3});;

}
