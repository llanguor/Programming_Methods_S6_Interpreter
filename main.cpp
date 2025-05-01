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

int add(std::vector<int> argv)
{
    int sum = 0;
    for (int x: argv)
    {
        sum+=x;
    }
    return sum;
}

int mult(std::vector<int> argv)
{
    int sum = 1;
    for (int x: argv)
    {
        sum*=x;
    }
    return sum;
}


int main()
{

    trie<std::function<int(std::vector<int>)>> funcs("abcdefghijklmnopqrstuvwxyz0123456789_");
    funcs.upsert("mult", mult);
    funcs.upsert("ad", add);


    auto interpreter = interpreter::builder(
        "../meta/input/settings.txt",
        1,
        "abcdefghijklmnopqrstuvwxyz0123456789_")
    .append_function("add", add)
    .append_function_map(funcs)
    .check_function("mult");
    std::cout<<interpreter;
    return 0;
}

