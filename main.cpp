#include <iostream>
#include "trie.h"
#include <string>
#include <memory>

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
    std::cout <<  t->obtain("keyba") << std::endl;
    std::cout <<  t->obtain("kkea") << std::endl;
    std::cout <<  t->obtain("kkek") << std::endl;
    std::cout <<  t->obtain("keyb") << std::endl;
    std::cout <<  t->obtain("keya") << std::endl;
    std::cout <<  t->obtain("key") << std::endl;
   // std::cout <<  t->obtain("keybc") << std::endl;


    std::map<char,size_t> m;
    m['a']=0;
    m['b']=0;
    m['c']=0;
    std::shared_ptr<int> ptr1 {std::make_shared<int>()};
    std::shared_ptr<int> ptr2 {ptr1};
    *ptr2=10;

    std::cout << "ptr1 address: " << ptr1 << std::endl;
    std::cout << "ptr1 value: " << *ptr1 << std::endl;
    std::cout << "ptr2 address: " << ptr2 << std::endl;
    std::cout << "ptr2 value: " << *ptr2 << std::endl;



    return 0;
}
