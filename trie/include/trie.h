#ifndef TRIE_H
#define TRIE_H

#include "associative_container.h"
#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <vector>
#include <optional>
#include <memory>


template<
    typename tvalue>
class trie final:
    public associative_container<std::string, tvalue>
{

private:

    struct node
    {

        std::vector<std::shared_ptr<node>> subtrees;
        std::optional<tvalue> value;

        explicit node(
            std::map<char, size_t> const &alphabet):
                subtrees(alphabet.size()),
                value(std::nullopt)
        {

        }
    };

private:

    std::shared_ptr<trie::node> _root;
    std::map<char, size_t> _alphabet_mapping;

public:

    explicit trie(
        std::string const &alphabet);

private:

    std::stack<std::shared_ptr<trie::node> *> find_path(
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

public:

    trie(
        trie const &other)
    = default;

    trie& operator=(
        trie const &other)
    = default;

    trie(
        trie &&other) noexcept
    = default;

    trie &operator=(
        trie &&other) noexcept
    = default;

    ~trie() noexcept override
    = default;

};

#include "trie.tpp"

#endif
