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


    std::cout<<"Value: "<< value.value() <<std::endl;

    //todo

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
    //todo

    auto const name = get_name_from_input();
    if (!get_variable_from_name(inter, name))
    {
        return;
    }

    auto value = get_value_from_input();

    try
    {
        inter._variables.upsert(name, std::move(value));
    }
    catch (std::out_of_range const e)
    {
        std::cout<<e.what()<<std::endl;
    }
}

void debugger::declare_variable(interpreter &inter)
{
    //todo

    auto const name = get_name_from_input();
    try
    {
        inter._variables.obtain(name);
        std::cout<<"Variable already exists"<<std::endl;
        return;
    }
    catch (std::out_of_range const e)
    {

    }

    auto value = get_value_from_input();
    try
    {
        inter._variables.upsert(name, std::move(value));
    }
    catch (std::out_of_range const e)
    {
        std::cout<<e.what()<<std::endl;
    }
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
