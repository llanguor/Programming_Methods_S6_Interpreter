#include <iostream>
#include "trie.h"
#include <string>

int main()
{
    std::string const alphabet = "abcdefghijklmnopqrsty";
    auto * t = new trie<int>(alphabet);
    t->upsert("", 100);
    t->upsert("key", 5);
    t->upsert("keya", 5);
    t->upsert("keyb", 6);
    t->upsert("kkek", 7);
    t->upsert("kkea", 8);
    t->upsert("keyba", 9);
    t->upsert("keybc", 10);
    t->dispose("keybc");
    int searched = t->obtain("keyba");
   std::cout << searched << std::endl;
    return 0;
}
