#pragma once
#include "interpreter.hpp"
#include "lexer.hpp"
#include <sstream>

class interpreter::builder
{

    std::fstream _stream;
    lexer _lexer;

public:

    interpreter::builder(
        std::string const & settings_file_path,
        int enclosure_max_level,
        std::regex const * regex_chars,
        std::regex const * regex_separators);

};