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



template<
    typename tvalue>
class trie final:
    public associative_container<std::string, tvalue>
{

private:

    struct node
    {

        std::vector<node *> subtrees;
        std::optional<tvalue> value;

        explicit node(
            std::map<char, size_t> const &alphabet):
                subtrees(alphabet.size()),
                value(std::nullopt)
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

#include "trie.tpp"

#endif
