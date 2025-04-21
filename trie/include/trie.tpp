#ifndef TRIE_TPP
#define TRIE_TPP

#include "trie.h"

template<
    typename tvalue>
trie<tvalue>::trie(
    std::string const &alphabet)
{
    if (alphabet.empty())
    {
        throw std::out_of_range("empty alphabet passed");
    }

    std::set<char> alphabet_items;
    for (auto a: alphabet)
    {
        alphabet_items.insert(a);
    }

    if (alphabet_items.size() != alphabet.length())
    {
        throw std::out_of_range("similar elements found in alphabet");
    }

    size_t i = 0;
    for (auto a: alphabet_items)
    {
        _alphabet_mapping[a] = i++;
    }

    _root.reset(new node(_alphabet_mapping));
}

template<
    typename tvalue>
std::stack<typename std::shared_ptr<typename trie<tvalue>::node>*> trie<tvalue>::find_path(
    std::string const &key)
{
    std::stack<std::shared_ptr<trie::node>*> path;
    auto * current = &_root;
    path.push(&_root);

    for (auto k: key)
    {
        auto it = _alphabet_mapping.find(k);
        if (it == _alphabet_mapping.end())
        {
            throw std::out_of_range("character is not contained in alphabet");
        }

        // THIS IS BAD
        // current = current->subtrees[it->second];
        // path.push(&current)

        path.push(&current->get()->subtrees[it->second]);
        current = &current->get()->subtrees[it->second];
        if (*current == nullptr)
        {
            return path;
        }
    }

    return path;
}

template<
    typename tvalue>
void trie<tvalue>::upsert(
    std::string const &key,
    tvalue &&value)
{
    auto current_node = &_root;

    for (auto k: key)
    {
        auto it = _alphabet_mapping.find(k);
        if (it == _alphabet_mapping.end())
        {
            throw std::out_of_range("character is not contained in alphabet");
        }

        if (current_node->get()->subtrees[it->second] == nullptr)
        {
            current_node->get()->subtrees[it->second].reset(new node(_alphabet_mapping));
        }

        current_node = &current_node->get()->subtrees[it->second];
    }

    current_node->get()->value = std::forward<tvalue>(value);
}

template<
    typename tvalue>
tvalue &trie<tvalue>::obtain(
    std::string const &key)
{
    auto path = find_path(key);
    if (*path.top() == nullptr || !(*path.top())->value)
    {
        throw std::out_of_range("key is not contained in trie");
    }
    return *((*path.top())->value);
}

template<
    typename tvalue>
tvalue trie<tvalue>::dispose(
    std::string const &key)
{
    auto path = find_path(key);
    auto current_node = path.top();
    if (*current_node == nullptr || !(*current_node)->value)
    {
        throw std::out_of_range("key is not contained in trie");
    }

    auto result_value = *(*current_node)->value;
    (*current_node)->value=std::nullopt;

    //Because if path.size==1 then path.top()==&_root
    while (path.size()>1)
    {
        for (auto &subtree: (*current_node)->subtrees)
        {
            if (subtree != nullptr)
                return result_value;
        }

        if (!(*current_node)->value)
        {
            return result_value;
        }

        current_node->reset();

        path.pop();
        current_node = path.top();
    }

    return result_value;
}


#endif
