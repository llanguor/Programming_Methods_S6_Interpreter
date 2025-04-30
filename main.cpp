#include <iostream>
#include "trie.h"
#include <string>
#include <memory>

#include "comments_handler.h"

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
    std::cout<<"End infix\n";


    std::istringstream ss("He#l\nlo,[# world!\nas]d");
    auto a = comments_handler(&ss, 1);

    for (int ch: a)
    {
        std::cout << static_cast<char>(ch);
    }

    return 0;
}
