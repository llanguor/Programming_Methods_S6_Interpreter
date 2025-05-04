#include <iostream>
#include "trie.hpp"
#include <string>
#include <memory>
#include <sstream>
#include<filesystem>
#include "interpreter.hpp"
#include "builder.hpp"
#include "operations.hpp"
#include "comments_handler.hpp"
#include "tokenizer.hpp"


int main()
{
    std::string const path = "../meta/input/";

    auto functions_trie = operations::get_functions_trie();

    auto interpreter =
        interpreter::builder<interpreter_rpn>(path+"settings.txt")
    .append_function_map(functions_trie)
    .set_debug_mode(true)
    .build();

    interpreter->run(path+"program1.txt");

    delete interpreter;
    /*
    std::stringstream ss1 ("asdas#DEBUGd\nd");
    comments_handler asd (&ss1, 1);
    auto it = asd.begin_char_only();
    while (it!=asd.end_char_only())
    {
        auto aaaa = *it;
        std::cout << static_cast<char>( aaaa);
        ++it;
    }
    */

    /*
    tokenizer tok (settings_path, 1,R"([\r\n\t ]+)",R"([^\r\n\t ])", true);
    auto it = tok.begin_string_only();
    while (it!=tok.end_string_only())
    {
        auto aaaa = *it;
        std::cout <<  aaaa << " ";
        ++it;
    }
    */
    return 0;
}

