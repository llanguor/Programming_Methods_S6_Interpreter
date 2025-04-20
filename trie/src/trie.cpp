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
    if (*path.top() == nullptr || (*path.top())->value == nullptr)
    {
        throw std::out_of_range("key is not contained in trie");
    }

    auto result_value = (*path.top())->value;
    delete (*path.top())->value;



    return result_value;
}