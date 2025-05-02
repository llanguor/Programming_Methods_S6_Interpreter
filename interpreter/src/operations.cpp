#include "operations.hpp"

#include <stdexcept>

#pragma region operations_class implementation

std::string const operations::_default_operations_alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_=";

void operations::throw_if_unexpected_size(
    unsigned long long const & current_size,
    unsigned long long const &expected_size)
{
    if (current_size!=expected_size)
    {
        throw std::invalid_argument(
            std::format(
                "Invalid number of arguments: {} arguments expected", expected_size));
    }
}

trie<std::function<int(std::vector<int>)>>
operations::get_functions_trie(
    std::string const & alphabet)
{
    trie<std::function<int(std::vector<int>)>> functions_trie(alphabet);
    functions_trie.upsert("not", operations::_not);
    functions_trie.upsert("input", operations::input);
    functions_trie.upsert("output", operations::output);
    functions_trie.upsert("add", operations::add);
    functions_trie.upsert("mult", operations::mult);
    functions_trie.upsert("sub", operations::sub);
    functions_trie.upsert("pow", operations::pow);
    functions_trie.upsert("div", operations::div);
    functions_trie.upsert("rem", operations::rem);
    functions_trie.upsert("xor", operations::_xor);
    functions_trie.upsert("and", operations::_and);
    functions_trie.upsert("or", operations::_or);
    functions_trie.upsert("=", operations::assignment);
    return functions_trie;
}

#pragma endregion

#pragma region unary

int operations::_not(std::vector<int> const &argv)
{
    throw_if_unexpected_size(argv.size(), 1);
    return ~argv[0];
}

int operations::input(std::vector<int> const &argv)
{
    throw_if_unexpected_size(argv.size(), 1);
    int base_input = argv[0];

    if (base_input < 2 || base_input > 36)
    {
        throw std::invalid_argument("Base must be between 2 and 36.");
    }

    std::cout << "Enter a number in base " << base_input << ": ";
    std::string input_value;
    std::cin >> input_value;

    int result = 0;
    int power = 1;

    for (auto it = input_value.rbegin(); it != input_value.rend(); ++it)
    {
        char digit = *it;
        int digit_value;

        if (digit >= '0' && digit <= '9')
        {
            digit_value = digit - '0';
        }
        else if (digit >= 'a' && digit <= 'z')
        {
            digit_value = digit - 'a' + 10;
        }
        else if (digit >= 'A' && digit <= 'Z')
        {
            digit_value = digit - 'A' + 10;
        }
        else
        {
            throw std::invalid_argument("Invalid character in input.");
        }

        if (digit_value >= base_input)
        {
            throw std::invalid_argument("Digit out of range for the given base.");
        }

        result += digit_value * power;
        power *= base_input;
    }

    return result;
}

int operations::output(std::vector<int> const &argv)
{
    throw_if_unexpected_size(argv.size(), 2);
    int value = argv[0];
    int base_output = argv[1];

    if (base_output < 2 || base_output > 36)
    {
        throw std::invalid_argument("Base must be between 2 and 36.");
    }

    if (value < 0)
    {
        throw std::invalid_argument("Value must be non-negative.");
    }

    std::string result;
    if (value == 0)
    {
        result = "0";
    }
    else
    {
        while (value > 0)
        {
            int digit = value % base_output;
            result.push_back(
                digit < 10 ?
                '0' + digit :
                'a' + (digit - 10));
            value /= base_output;
        }

        std::reverse(result.begin(), result.end());
    }

    std::cout << "Output in base " << base_output << ": " << result << std::endl;
    return 0;
}

#pragma endregion

#pragma region binary

int operations::add(std::vector<int> const &argv)
{
    throw_if_unexpected_size(argv.size(), 2);
    return argv[0] + argv[1];
}

int operations::mult(std::vector<int> const &argv)
{
    throw_if_unexpected_size(argv.size(), 2);
    return argv[0] * argv[1];
}

int operations::sub(std::vector<int> const &argv)
{
    throw_if_unexpected_size(argv.size(), 2);
    return argv[0] - argv[1];
}

int operations::pow(std::vector<int> const &argv)
{
    throw_if_unexpected_size(argv.size(), 2);

    int base = argv[0],
        exp = argv[1],
        result = 1;

    while (exp > 0)
    {
        if (exp % 2 == 1)
            result = (result * base) % 2;
        base = (base * base) % 2;
        exp /= 2;
    }

    return result;
}

int operations::div(std::vector<int> const &argv)
{
    throw_if_unexpected_size(argv.size(), 2);

    if (argv[1] == 0)
        throw std::invalid_argument("Divide by zero.");

    return argv[0] / argv[1];
}

int operations::rem(std::vector<int> const &argv)
{
    throw_if_unexpected_size(argv.size(), 2);

    if (argv[1] == 0)
        throw std::invalid_argument("Divide by zero.");

    return argv[0] % argv[1];
}

int operations::_xor(std::vector<int> const &argv)
{
    throw_if_unexpected_size(argv.size(), 2);
    return argv[0] ^ argv[1];
}

int operations::_and(std::vector<int> const &argv)
{
    throw_if_unexpected_size(argv.size(), 2);
    return argv[0] & argv[1];
}

int operations::_or(std::vector<int> const &argv)
{
    throw_if_unexpected_size(argv.size(), 2);
    return argv[0] | argv[1];
}

int operations::assignment(std::vector<int> const &argv)
{
    return 0;
}

#pragma endregion
