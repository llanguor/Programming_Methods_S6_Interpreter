#include <iostream>
#include "trie.h"
#include <string>
#include <memory>

int main()
{
    std::string const alphabet = "abcdefghijklmnopqrsty";
    auto t (std::make_shared<trie<int>>(alphabet));
    t->upsert("", 1);
    t->upsert("keybc", 6);
    t->upsert("kkea", 5);
    t->upsert("kkek", 4);
    t->upsert("kya", 2);
    t->upsert("kyy", 3);
    t->upsert("kyya", 3);
    t->upsert("kyyy", 3);
    t->dispose("kyya");



    for (auto const & inf: *t)
    {
        std::cout<<"Node '" << inf->key <<"' = "<< inf->value << std::endl;
    }
    std::cout<<"End infix";

    return 0;
}
