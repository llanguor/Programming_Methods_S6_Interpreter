#pragma once
#include <vector>
#include <format>
#include <iostream>
#include <algorithm>
#include "trie.hpp"
#include <functional>

class operations final
{

public:

    static std::string const default_operations_alphabet;

public:

    static trie<std::function<int(std::vector<int>)>> get_functions_trie(
        std::string const &alphabet = default_operations_alphabet);

public:

    static int _not(std::vector<int> const &argv);

    static int input(std::vector<int> const &argv);

    static int output(std::vector<int> const &argv);

public:

    static int add(std::vector<int> const &argv);

    static int mult(std::vector<int> const &argv);

    static int sub(std::vector<int> const &argv);

    static int pow(std::vector<int> const &argv);

    static int div(std::vector<int> const &argv);

    static int rem(std::vector<int> const &argv);

    static int _xor(std::vector<int> const &argv);

    static int _and(std::vector<int> const &argv);

    static int _or(std::vector<int> const &argv);

private:

    static void throw_if_unexpected_size(
        unsigned long long const &current_size,
        unsigned long long const &min_size,
        unsigned long long const &max_size);

    static void throw_if_unexpected_size(
        unsigned long long const &current_size,
        unsigned long long const &expected_size);

    static void throw_if_incorrect_base(
        size_t const & base);

public:

    static int base_to_decimal(
        std::string const &value, int const &base);

    static std::string decimal_to_base(
        int const &value, int const &base);

};
