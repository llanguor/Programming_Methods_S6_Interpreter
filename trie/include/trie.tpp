#pragma once
#include <utility>
#include "trie.hpp"


#pragma region trie

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

        if ((*current_node)->value)
        {
            return result_value;
        }

        current_node->reset();

        path.pop();
        current_node = path.top();
    }

    return result_value;
}

#pragma endregion

#pragma region iterator_data

template<typename tvalue>
trie<tvalue>::iterator_data::iterator_data(
    unsigned int depth,
    std::string const & key,
    tvalue const &value):
    depth(depth),
    key(key),
    value(value)
{

}

#pragma endregion

#pragma region iterator_base

template<typename tvalue>
trie<tvalue>::iterator_base::iterator_base(
    std::map<char, size_t> const &alphabet,
    typename trie<tvalue>::node *subtree_root):
    _alphabet_mapping(alphabet)
{
    for (const auto &k: alphabet)
    {
        _alphabet_reverse[k.second] = k.first;
    }

    _stack.push(subtree_root);
}

template<typename tvalue>
typename trie<tvalue>::iterator_data * trie<tvalue>::iterator_base::operator*() const
{
    if (_stack.top()==nullptr)
    {
        throw std::out_of_range("Attempted to access past-the-end element");
    }

    return new iterator_data(
        iterator_base::_stack.size()-1,
        _assembled_key,
        iterator_base::_stack.top()->value.value());
}


template<typename tvalue>
bool trie<tvalue>::iterator_base::operator==(iterator_base const &other) const noexcept
{
    return other._stack.top() == this->_stack.top();
}

template<typename tvalue>
bool trie<tvalue>::iterator_base::operator!=(iterator_base const &other) const noexcept
{
    return other._stack.top() != this->_stack.top();
}

template<typename tvalue>
void trie<tvalue>::iterator_base::fall_to_next_bottom()
{
    node * current_node = iterator_base::_stack.top();
    node * start_node = current_node;

    size_t subtree_i;
    while (subtree_i != iterator_base::_alphabet_reverse.size())
    {
        subtree_i = 0;

        if (current_node->value!=std::nullopt &&
            current_node != start_node)
        {
            return;
        }

        for (auto subtree: current_node->subtrees)
        {
            if (subtree != nullptr)
            {
                current_node = &*subtree;
                iterator_base::_stack.push(&*subtree);
                iterator_base::_assembled_key += iterator_base::_alphabet_reverse[subtree_i];
                break;
            }

            ++subtree_i;
        }
    }
}

#pragma endregion

#pragma region infix_iterator

template<typename tvalue>
trie<tvalue>::infix_iterator::infix_iterator(
    std::map<char, size_t> const & alphabet,
    typename trie<tvalue>::node *subtree_root):
    iterator_base(alphabet, subtree_root)
{
    if (iterator_base::_stack.top()==nullptr)
    {   //end_infix
        return;
    }

    if (iterator_base::_stack.top()->value != std::nullopt)
    {
        return;
    }

    iterator_base::fall_to_next_bottom();

    if (iterator_base::_stack.top()->value==std::nullopt)
    {
        //all subtrees is nullptr and value is nullpt => tree is empty
        iterator_base::_stack.push(nullptr);
    }
}

template<typename tvalue>
typename trie<tvalue>::infix_iterator & trie<tvalue>::infix_iterator::operator++()
{
    node * start_node = iterator_base::_stack.top();
    if (iterator_base::_stack.top()==nullptr)
    {   //end_infix
        return *this;
    }

    iterator_base::fall_to_next_bottom();
    if (iterator_base::_stack.top() != start_node)
    {
        return *this;
    }


    while (!iterator_base::_stack.empty())
    {
        iterator_base::_stack.pop();

        if (iterator_base::_stack.empty())
        {
            //if after pop we get an empty array, then either there are no other nodes
            //or we have already walked the tree (returned from the right)
            iterator_base::_stack.push(nullptr);
            return *this;
        }

        node * current_node = iterator_base::_stack.top();
        auto current_subtree_id = iterator_base::_alphabet_mapping.at(
            iterator_base::_assembled_key.back());

        iterator_base::_assembled_key.pop_back();

        for (int i=current_subtree_id+1; i<iterator_base::_alphabet_mapping.size(); ++i)
        {
            auto subtree = & current_node->subtrees[i];
            if (*subtree != nullptr)
            {
                current_node = &**subtree;
                iterator_base::_stack.push(current_node);
                iterator_base::_assembled_key += iterator_base::_alphabet_reverse[i];

                if (current_node->value == std::nullopt)
                {
                    iterator_base::fall_to_next_bottom();
                }

                return *this;
            }
        }
    }

    return *this;
}

template<typename tvalue>
typename trie<tvalue>::infix_iterator trie<tvalue>::infix_iterator::operator++(int not_used)
{
    infix_iterator temp {*this};
    ++(*this);
    return temp;
}

template<typename tvalue>
typename trie<tvalue>::infix_iterator trie<tvalue>::begin() const noexcept
{
    return trie<tvalue>::infix_iterator(
        _alphabet_mapping,
        dynamic_cast<typename trie<tvalue>::node *>(&*_root)
        );
}

template<typename tvalue>
typename trie<tvalue>::infix_iterator trie<tvalue>::end() const noexcept
{
    return trie<tvalue>::infix_iterator(
        _alphabet_mapping,
        nullptr);
}

template<typename tvalue>
typename trie<tvalue>::iterator_data * trie<tvalue>::infix_iterator::operator*() const
{
    return iterator_base::operator*();
}

template<typename tvalue>
bool trie<tvalue>::infix_iterator::operator!=(infix_iterator const &other) const noexcept
{
    return iterator_base::operator!=(other);
}

template<typename tvalue>
bool trie<tvalue>::infix_iterator::operator==(infix_iterator const &other) const noexcept
{
    return iterator_base::operator==(other);
}

#pragma endregion