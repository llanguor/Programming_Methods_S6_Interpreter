#include <iostream>
#include "trie.hpp"
#include <string>
#include <memory>
#include <sstream>
#include<filesystem>
#include "interpreter_rpn.hpp"
#include "builder.hpp"
#include "operations.hpp"
#include "comments_handler.hpp"
#include "tokenizer.hpp"


int main()
{

    std::string const path = "../meta/input/";

    auto functions_trie = operations::get_functions_trie();

    auto interpreter =
        interpreter::builder<interpreter_rpn>(path+"settings3.txt")
    .append_function_map(functions_trie)
    .set_debug_mode(true)
    .set_base_assign(10)
    .build();
   // try
    {
        interpreter->run(path+"program3.txt");
    }
   // catch(const std::exception& e)
    {
        //std::cout<<e.what()<<std::endl;
    }

    delete interpreter;



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

