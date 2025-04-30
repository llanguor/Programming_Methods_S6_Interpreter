#pragma once
#include "trie.hpp"

class interpreter
{

    trie<int> _variables;
    //trie _functions
    size_t _base_assign = 10, _base_input=10, _base_output=10;


public:

    class builder;
};