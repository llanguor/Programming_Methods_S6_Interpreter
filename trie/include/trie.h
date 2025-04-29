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


public:

    struct iterator_data
    {

    public:

        unsigned int depth;

        std::string const key;

        tvalue value;

    public:

        explicit iterator_data(
            unsigned int depth,
            std::string const & key,
            tvalue const &value);
    };

public:

    class iterator_base
    {

    protected:

        std::string _assembled_key;

        std::stack<node*> _stack;

        std::map<size_t, char> _alphabet_reverse;

        std::map<char, size_t> const & _alphabet_mapping;

    protected:

        explicit iterator_base(
            std::map<char, size_t> const & alphabet,
            typename trie<tvalue>::node *subtree_root);

        bool operator==(
            iterator_base const &other) const noexcept;

        bool operator!=(
            iterator_base const &other) const noexcept;

        iterator_data *operator*() const;

        void fall_to_next_bottom();
    };


    class infix_iterator final:iterator_base
    {

    public:

        explicit infix_iterator(
            std::map<char, size_t> const & alphabet,
            typename trie<tvalue>::node *subtree_root);

    public:

        bool operator==(
            infix_iterator const &other) const noexcept;

        bool operator!=(
            infix_iterator const &other) const noexcept;

        infix_iterator &operator++();

        infix_iterator operator++(
            int not_used);

        iterator_data *operator*() const;


    };

public:

    infix_iterator begin() const noexcept;

    infix_iterator end() const noexcept;

};

#include "trie.tpp"

#endif
