#pragma once
#include <fstream>
#include <regex>
#include <string>
#include <optional>
#include <utility>
#include "operations.hpp"
#include "interpreter.hpp"
#include "tokenizer.hpp"

template<typename interpreter_type>
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
    size_t _comments_enclosure_max_level=1;
    bool _is_debug_mode_enabled = false;
    std::string _variables_alphabet = interpreter::default_variables_alphabet;

private:
    //init by file parse
    lvalues_position_types _lvalues_position = left;
    arguments_position_types _arguments_position = after_operation;


public:

    explicit builder(
        std::string const & settings_file_path);

    explicit builder(
        std::string const & settings_file_path,
        std::string const & operations_alphabet);

public:

    builder& append_function(
        std::string const & original_name,
        std::function<int(std::vector<int>)> function);

    builder& append_function_map(
        trie<std::function<int(std::vector<int>)>> const & functions_map);

    builder& set_base_assign(
        size_t const & base);

    builder& set_base_input(
        size_t const & base);

    builder& set_base_output(
        size_t const & base);

    builder& set_comments_enclosure_max_level(
       size_t const & max_level);

    builder& set_debug_mode(
        bool const & is_debug_mode_enabled);

    builder& set_variables_alphabet(
         std::string const & alphabet);

public:

    interpreter_type * build();

};

#include "builder.tpp"