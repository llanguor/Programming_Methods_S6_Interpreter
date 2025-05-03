#include "interpreter.hpp"

std::string const interpreter::_default_variables_alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";

interpreter::interpreter(

    trie<std::function<int(std::vector<int>)>> const &operations,
    lvalues_position_types const &lvalues_position,
    arguments_position_types const &arguments_position,
    size_t const &base_assign,
    size_t const &base_input,
    size_t const &base_output,
    bool const &is_debug_mode_enabled,
    std::string const & variables_alphabet):

    _operations(operations),
    _lvalues_position(lvalues_position),
    _arguments_position(arguments_position),
    _base_assign(base_assign),
    _base_input(base_input),
    _base_output(base_output),
    _is_debug_mode_enabled(is_debug_mode_enabled),
    _variables(variables_alphabet)

{

}
