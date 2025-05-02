#pragma once
#include "interpreter.hpp"
#include "lexer.hpp"
#include <fstream>
#include <regex>
#include <string>
#include "operations.hpp"

class interpreter::builder
{
    size_t _base_assign, _base_input, _base_output;
    bool _is_debug;
    std::fstream _stream;
    lexer _lexer;
    lvalues_position_types const _lvalues_position = left;
    arguments_position_types const _arguments_position = after_operation;
    std::string const _variables_alphabet;

    //for program file
    std::regex _regex_chars;
    std::regex _regex_separators;

    trie<std::function<int(std::vector<int>)>> _functions_map;

public:

    explicit builder(
        std::string const & settings_file_path,
        int enclosure_max_level = 1,
        size_t base_assign=10,
        size_t base_input=10,
        size_t base_output=10,
        bool is_debug=false,
        std::string variables_alphabet= interpreter::_default_variables_alphabet,
        std::string const & functions_alphabet= operations::_default_operations_alphabet);

    builder& append_function(
        std::string const & original_name,
        std::function<int(std::vector<int>)> function);

    auto append_function_map(
        trie<std::function<int(std::vector<int>)> > &functions_map) -> builder &;

    int check_function(
        std::string const & original_name);


};