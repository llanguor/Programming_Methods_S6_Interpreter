#ifndef TRIE_H
#define TRIE_H

#include "associative_container.h"
#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <vector>
#include <optional>
#include <unordered_map>


// std::optional<T>
// std::variant<T>


template<
    typename tvalue>
class trie final:
    public associative_container<std::string, tvalue>
{

private:

    struct node
    {

        std::vector<node *> subtrees;
        tvalue *value;

        explicit node(
            std::map<char, size_t> const &alphabet):
                subtrees(alphabet.size()),
                value(nullptr)
        {
            for (auto &s: subtrees)
            {
                s = nullptr;
            }
        }

    };

private:

    trie::node *_root;
    std::map<char, size_t> _alphabet_mapping;

public:

    explicit trie(
        std::string const &alphabet);

public:

    trie(
        trie const &other);

    trie& operator=(
        trie const &other);

    trie(
        trie &&other) noexcept;

    trie &operator=(
        trie &&other) noexcept;

    ~trie() noexcept override;

private:

    std::stack<trie<tvalue>::node **> find_path(
        std::string const &key);

public:

    void upsert(
        std::string const &key,
        tvalue &&value)
    override;

    tvalue &obtain(
        std::string const &key)
    override;

    tvalue dispose(
        std::string const &key)
    override;

};





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

    _root = new node(_alphabet_mapping);
}

template<
    typename tvalue>
trie<tvalue>::trie(
    trie<tvalue> const &other)
{

}

template<
    typename tvalue>
trie<tvalue>& trie<tvalue>::operator=(
    trie<tvalue> const &other)
{

}

template<
    typename tvalue>
trie<tvalue>::trie(
    trie<tvalue> &&other) noexcept
{

}

template<
    typename tvalue>
trie<tvalue>& trie<tvalue>::operator=(
    trie<tvalue> &&other) noexcept
{

}

template<
    typename tvalue>
trie<tvalue>::~trie() noexcept
{

}

template<
    typename tvalue>
std::stack<typename trie<tvalue>::node **> trie<tvalue>::find_path(
    std::string const &key)
{
    std::stack<trie<tvalue>::node **> path;
    auto *current = _root;

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
        // path.push(&current);

        path.push(&current->subtrees[it->second]);
        if ((current = current->subtrees[it->second]) == nullptr)
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
    auto *current_node = _root;

    for (auto k: key)
    {
        auto it = _alphabet_mapping.find(k);
        if (it == _alphabet_mapping.end())
        {
            throw std::out_of_range("character is not contained in alphabet");
        }

        if (current_node->subtrees[it->second] == nullptr)
        {
            current_node->subtrees[it->second] = new node(_alphabet_mapping);
        }

        current_node = current_node->subtrees[it->second];
    }

    if (current_node->value == nullptr)
    {
        current_node->value = new tvalue(std::forward<tvalue>(value));
    }
    else
    {
        *(current_node->value) = std::forward<tvalue>(value);
    }
}

template<
    typename tvalue>
tvalue &trie<tvalue>::obtain(
    std::string const &key)
{
    auto path = find_path(key);
    if (*path.top() == nullptr || (*path.top())->value == nullptr)
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
    if (*current_node == nullptr || (*current_node)->value == nullptr)
    {
        throw std::out_of_range("key is not contained in trie");
    }

    auto result_value = *(*current_node)->value;
    delete (*current_node)->value;
    (*current_node)->value=nullptr;

    //Because if path.size==1 then path.top()==&_root
    while (path.size()>1)
    {
        for (auto subtree: (*current_node)->subtrees)
        {
            if (subtree != nullptr)
                return result_value;
        }

        if ((*current_node)->value!=nullptr)
        {
            return result_value;
        }

        delete *current_node;
        *current_node = nullptr;

        path.pop();
        current_node = path.top();
    }

    return result_value;
}

#endif
