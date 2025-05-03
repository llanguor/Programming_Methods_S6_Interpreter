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
    const auto settings_path = "../meta/input/settings.txt";
    auto functions_trie = operations::get_functions_trie();

    auto interpreter =
        interpreter::builder(settings_path)
    .append_function_map(functions_trie)
    .build();

    std::stringstream ss1 ("asdas#DEBUGd\nd");
    comments_handler asd (&ss1, 1);
    auto it = asd.begin_char_only();
    while (it!=asd.end_char_only())
    {
        auto aaaa = *it;
        std::cout << static_cast<char>( aaaa);
        ++it;
    }
    return 0;
}

