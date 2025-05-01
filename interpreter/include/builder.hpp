#pragma once
#include "interpreter.hpp"
#include "lexer.hpp"
#include <fstream>
#include <regex>
#include <string>

class interpreter::builder
{

    std::fstream _stream;
    lexer _lexer;

    //for program file
    std::regex _regex_chars;
    std::regex _regex_separators;

    trie<std::function<int(std::vector<int>)>> _functions_map;

public:

    builder(
        std::string const & settings_file_path,
        int const enclosure_max_level,
        std::string const & variables_alphabet);

    builder& append_function(
        std::string const & original_name,
        std::function<int(std::vector<int>)> function);

    auto append_function_map(
        trie<std::function<int(std::vector<int>)> > &functions_map) -> builder &;

    int check_function(
        std::string const & original_name);


};