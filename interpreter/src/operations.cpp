#include "operations.hpp"
#include <stdexcept>


#pragma region operations_class implementation

std::string const operations::default_operations_alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_=/%<>-";

void operations::throw_if_incorrect_base(size_t const & base)
{
    if (base < 2 || base > 36)
    {
        throw std::invalid_argument("Base must be between 2 and 36.");
    }
}


void operations::throw_if_unexpected_size(
    unsigned long long const & current_size,
    unsigned long long const & min_size,
    unsigned long long const & max_size)
{
    if (current_size<min_size || current_size>max_size)
    {
        std::string msg_part =
            (min_size==max_size)?
            std::to_string(min_size):
            std::format("from {} to {}", min_size, max_size);

        throw std::invalid_argument(
                "Invalid number of arguments: "+ msg_part +" arguments expected");
    }
}

void operations::throw_if_unexpected_size(
    unsigned long long const & current_size,
    unsigned long long const &expected_size)
{
    throw_if_unexpected_size(current_size, expected_size, expected_size);
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
    return functions_trie;
}


int operations::base_to_decimal(
    std::string const &value, int const &base)
{
    std::size_t pos = 0;
    throw_if_incorrect_base(base);

    try
    {
        auto result = std::stoi(value, &pos, base);
        if (pos != value.size())
        {
            throw std::invalid_argument("The input data contains an invalid value: '" + value.substr(pos) + "'");
        }
        return result;
    }
    catch (std::invalid_argument const &e)
    {
        throw std::invalid_argument("Invalid data for base conversion: " + value);
    }
    catch (std::out_of_range const &e)
    {
        throw std::out_of_range("Value out of range for base conversion: " + value);
    }
}

std::string operations::decimal_to_base(
    int const &value, int const & base)
{
        throw_if_incorrect_base(base);

        if (value == 0) return "0";

        bool negative = value < 0;
        unsigned int n = std::abs(value);
        std::string result;

        while (n > 0)
        {
            int digit = n % base;
            char c = digit < 10 ? ('0' + digit) : ('A' + digit - 10);
            result += c;
            n /= base;
        }

        if (negative) result += '-';
        std::ranges::reverse(result);
        return result;
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

    throw_if_unexpected_size(argv.size(), 0, 1);
    int base_input = argv.size()==0 ? 10 : argv[0];
    throw_if_incorrect_base(base_input);

    std::cout << "Enter a number in base " << base_input << ":\n";
    std::string input_value;
    std::cin >> input_value;

    return base_to_decimal(input_value, base_input);
}

#pragma endregion

#pragma region binary

int operations::output(std::vector<int> const &argv)
{
    throw_if_unexpected_size(argv.size(), 1, 2);
    int value = argv[0];
    int base_output = argv.size()==1? 10 : argv[1];
    throw_if_incorrect_base(base_output);

    std::cout << "Output in base " << base_output << ": " << decimal_to_base(value, base_output) << std::endl;
    return 0;
}

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

#include <cstdint>  // Для использования uint64_t

int operations::pow(std::vector<int> const &argv)
{
    throw_if_unexpected_size(argv.size(), 2);

    int base = argv[0];
    int exp = argv[1];

    if (base == 0 && exp == 0)
        throw std::invalid_argument("0^0 is undefined");

    if (exp < 0)
        throw std::invalid_argument("Negative exponents are not supported");

    unsigned int result = 1;
    unsigned int b = static_cast<unsigned int>(base);

    uint64_t const MOD = 0x100000000; // 2^32, теперь используем uint64_t

    while (exp > 0)
    {
        if (exp % 2 == 1)
            result = static_cast<unsigned int>((result * b) % MOD);

        b = static_cast<unsigned int>((b * b) % MOD);
        exp /= 2;
    }

    return static_cast<int>(result);
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

#pragma endregion