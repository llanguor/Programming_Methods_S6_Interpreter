#pragma once
#include "interpreter.hpp"
#include "lexer.hpp"
#include <fstream>
#include <regex>
#include <string>
#include "operations.hpp"

class interpreter::builder
{

private:
    //init by constructor
    std::fstream _stream;
    trie<std::function<int(std::vector<int>)>> _operations_map;

private:
    //init by setters
    size_t _base_assign=10;
    size_t _base_input=10;
    size_t _base_output=10;
    bool _is_debug_mode_enabled = false;
    std::string _variables_alphabet = interpreter::_default_variables_alphabet;

private:
    //init by build
    std::string const _regex_string = "[a-zA-Z0-9_]";
    std::string const _regex_separators = "[\r\n\t ]+";
    std::unique_ptr<lexer> _lexer; //(&_stream, enclosure_max_level, _regex_string, _regex_separators)

private:
    //init by file parse
    lvalues_position_types const _lvalues_position = left;
    arguments_position_types const _arguments_position = after_operation;


public:

    explicit builder(
        std::string const & settings_file_path);

    explicit builder(
        std::string const & settings_file_path,
        std::string const & operations_alphabet);

public:

    int check_function(
        std::string const & original_name);

    builder& append_function(
        std::string const & original_name,
        std::function<int(std::vector<int>)> function);

    builder& append_function_map(
        trie<std::function<int(std::vector<int>)>> const &functions_map);

    builder& set_base_assign(
        size_t base);

    builder& set_base_input(
        size_t base);

    builder& set_base_output(
        size_t base);

    builder& set_debug_mode(
        bool is_debug_mode_enabled);

    builder& set_variables_alphabet(
         std::string const & alphabet);

public:

    interpreter const * build() const;


/*
    size_t _base_assign=10;
    size_t _base_input=10;
    size_t _base_output=10;
    bool _is_debug = false;
    std::string _variables_alphabet = interpreter::_default_variables_alphabet;
    */

};