#include <iostream>
#include "trie.hpp"
#include <string>
#include <memory>
#include <sstream>

#include "comments_handler.hpp"
#include "lexer.hpp"
#include<filesystem>
#include "interpreter.hpp"
#include "builder.hpp"
#include "operations.hpp"


int main()
{
    auto settings_path = "../meta/input/settings.txt";
    auto functions_trie = operations::get_functions_trie();

    auto interpreter =
        interpreter::builder(settings_path)
        .append_function_map(functions_trie)
        .check_function("div");

    std::cout<<interpreter;
    return 0;
}

