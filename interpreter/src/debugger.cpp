#include "../include/debugger.hpp"

bool debugger::handle_breakpoint(interpreter &interpr)
{
    int input = 0;

    while (input!=6 && input!=7)
    {
        std::cout<<std::endl<<"--------- Interpreter debug menu ---------"<<std::endl;
        std::cout<<"Commands list:"<<std::endl;
        std::cout<<"1. Print variable with memory dump"<<std::endl;
        std::cout<<"2. Print all variables"<<std::endl;
        std::cout<<"3. Change the value of a variable"<<std::endl;
        std::cout<<"4. Declare a new variable"<<std::endl;
        std::cout<<"5. Undeclaring a variable"<<std::endl;
        std::cout<<"6. Continue execution of the interpreter"<<std::endl;
        std::cout<<"7. Terminate the interpreter"<<std::endl;
        std::cout<<"Enter number:"<<std::endl;
        std::cin>>input;

        switch (input)
        {
            case 1:
                print_variable(interpr);
                break;
            case 2:
                print_all_variables(interpr);
                break;
            case 3:
                change_variable(interpr);
                break;
            case 4:
                declare_variable(interpr);
                break;
            case 5:
                undeclare_variable(interpr);
                break;
            case 6:
                return true;

            case 7:
                return false;

            default:
                std::cout<<"Invalid input"<<std::endl;
                break;
        }
    }

    return true;
}

void debugger::print_variable(interpreter &inter)
{
    auto const value = get_variable_from_name(
            inter,
            get_name_from_input());

    if (value==std::nullopt)
    {
        return;
    }

    auto const base = get_base_from_input();
    if (base==std::nullopt)
    {
        return;
    }

    std::cout<<"Value: "<< operations::decimal_to_base(value.value(), base.value()) <<std::endl;
    std::cout<<"Memory dump: "<<dump_memory(value.value()) << std::endl;
}

void debugger::print_all_variables(interpreter &inter)
{
    std::cout<<"Variable list:"<<std::endl;
    for (auto const & variable : inter._variables)
    {
        std::cout<<variable.key<<"="<<variable.value<<std::endl;
    }
}

void debugger::change_variable(interpreter &inter)
{
    auto const name = get_name_from_input();
    if (!get_variable_from_name(inter, name))
    {
        return;
    }


    auto const base = get_base_from_input();
    if (base==std::nullopt)
    {
        return;
    }

    std::string value_str;
    std::cout << "Enter variable value on base :" << base.value() << std::endl;
    std::cin >> value_str;

    try
    {
        auto value = operations::base_to_decimal(value_str, base.value());
        inter._variables.upsert(name, std::move(value));
    }
    catch (std::out_of_range const e)
    {
        std::cout<<e.what()<<std::endl;
    }
}

void debugger::declare_variable(interpreter &inter)
{
    auto const name = get_name_from_input();
    if (!name.empty() && std::isdigit(name[0]))
    {
        std::cout<<"Incorrect variable name: first char can't be number"<<std::endl;
        return;
    }

    try
    {
        inter._variables.obtain(name);
        std::cout<<"Variable already exists"<<std::endl;
        return;
    }
    catch (std::out_of_range const e)
    {

    }


    std::cout<<"Select input type:"<<std::endl;
    std::cout<<"1. From decimal value"<<std::endl;
    std::cout<<"2. From Zeckendorf representation"<<std::endl;
    std::cout<<"3. From roman numerals"<<std::endl;
    int input;
    std::cin>>input;

    try
    {
        int result;
        switch (input)
        {
            case 1:
                result = declare_variable_from_decimal(inter);
                break;

            case 2:
                result = declare_variable_from_zeckendorf(inter);
                break;

            case 3:
                result = declare_variable_from_roman_numerals(inter);
                break;

            default:
                std::cout<<"Incorrect input type"<<std::endl;
                return;
        }

        inter._variables.upsert(name, std::move(result));
    }
    catch (std::exception const & e)
    {
        std::cout<<e.what()<<std::endl;
    }

}

int debugger::declare_variable_from_decimal(interpreter &inter)
{
    return get_value_from_input();

}

int debugger::declare_variable_from_zeckendorf(interpreter &inter)
{
    //1010 => 10
    //110101 => 62

    std::string zeckendorf_str;
    std::cout << "Enter Zeckendorf representation value:"<<std::endl;
    std::cin >> zeckendorf_str;

    if (zeckendorf_str.empty())
    {
        throw std::invalid_argument("Input cannot be empty.");
    }

    for (char c : zeckendorf_str)
    {
        if (c != '0' && c != '1')
        {
            throw std::invalid_argument("Invalid character in Zeckendorf representation: " + std::string(1, c));
        }
    }

    if (zeckendorf_str.front() == '0')
    {
        throw std::invalid_argument("Zeckendorf representation cannot start with a leading zero.");
    }

    int result = 0;
    int power = 0;

    for (int i = zeckendorf_str.size() - 1; i >= 0; --i)
    {
        if (zeckendorf_str[i] == '1')
        {
            result += std::pow(2, power);
        }
        ++power;
    }

    return result;
}

int debugger::declare_variable_from_roman_numerals(interpreter &inter)
{
    std::string roman;
    std::cout << "Enter Roman numeral:" <<std::endl;
    std::cin >> roman;

    if (roman.empty())
    {
        throw std::invalid_argument("Input cannot be empty");
    }

    std::unordered_map<char, int> roman_map = {
        {'I', 1}, {'V', 5}, {'X', 10}, {'L', 50}, {'C', 100}, {'D', 500}, {'M', 1000}
    };

    for (char c : roman)
    {
        if (roman_map.find(c) == roman_map.end())
        {
            throw std::invalid_argument("Invalid character in Roman numeral: " + std::string(1, c));
        }
    }

    std::string valid_roman_numeral_regex = "^(M{0,3})(CM|CD|D?C{0,3})(XC|XL|L?X{0,3})(IX|IV|V?I{0,3})$";
    std::regex roman_regex(valid_roman_numeral_regex);
    if (!std::regex_match(roman, roman_regex))
    {
        throw std::invalid_argument("Invalid Roman numeral format.");
    }

    int result = 0;
    int prev_value = 0;

    for (int i = roman.length() - 1; i >= 0; --i)
    {
        char current_char = roman[i];
        int current_value = roman_map[current_char];

        if (current_value < prev_value)
        {
            result -= current_value;
        }
        else
        {
            result += current_value;
        }

        prev_value = current_value;
    }

    return result;
}

void debugger::undeclare_variable(interpreter &inter)
{
    auto const name = get_name_from_input();

    try
    {
        inter._variables.dispose(name);
    }
    catch (std::out_of_range const e)
    {
        std::cout<<e.what()<<std::endl;
    }

}

std::string debugger::get_name_from_input()
{
    std::string name;
    std::cout << "Enter variable name:" << std::endl;
    std::cin >> name;
    return name;
}

int debugger::get_value_from_input()
{
    int value;
    std::cout << "Enter variable value:" << std::endl;
    std::cin >> value;
    return value;
}

std::optional<int> debugger::get_base_from_input()
{
    int base;
    std::cout << "Enter base:" << std::endl;
    std::cin >> base;
    if (base<2 || base>32)
    {
        return std::nullopt;
    }
    return base;
}

std::optional<int> debugger::get_variable_from_name(interpreter &inter, std::string const & name)
{
    try
    {
        return inter._variables.obtain(name);
    }
    catch(std::out_of_range const e)
    {
        std::cout<<e.what()<<std::endl;
        return std::nullopt;
    }
}

std::string debugger::dump_memory(int const &value)
{
    unsigned char const *ptr = reinterpret_cast<unsigned char const *>(&value);
    std::stringstream ss;
    for (size_t i = 0; i < sizeof(value); ++i)
    {
        ss << std::bitset<8>(ptr[i]);
        if (i < sizeof(value) - 1)
        {
            ss << " ";
        }
    }

    return ss.str();
}