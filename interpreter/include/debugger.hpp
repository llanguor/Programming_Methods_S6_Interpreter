#pragma once
#include "interpreter.hpp"


class debugger final
{

public:

    static bool handle_breakpoint(interpreter & interpr);

private:

    static void print_variable(interpreter & inter);

    static void print_all_variables(interpreter const & inter);

    static void change_variable(interpreter & inter);

    static void undeclare_variable(interpreter & inter);

    static void declare_variable(interpreter & inter);

    static int declare_variable_from_decimal(interpreter &inter);

    static int declare_variable_from_zeckendorf(interpreter &inter);

    static int declare_variable_from_roman_numerals(interpreter &inter);

private:

    static std::string get_name_from_input();

    static int get_value_from_input();

    static std::optional<int> get_base_from_input();

    static std::optional<int> get_variable_from_name(interpreter &inter, std::string const & name);

    static std::string dump_memory(int const &value);
};
