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

public:

    builder(
        std::string const & settings_file_path,
        int const enclosure_max_level);

};