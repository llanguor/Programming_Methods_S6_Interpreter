#include <iostream>
#include "trie.hpp"
#include <string>
#include <memory>
#include <sstream>
#include<filesystem>
#include "interpreter_rpn.hpp"
#include "builder.hpp"
#include "operations.hpp"
#include "comments_handler.hpp"
#include "tokenizer.hpp"

std::string const last_settings_file_path = "../meta/input/last_settings.txt";

int main(int argc, char* argv[])
{
    //std::string const path = "../meta/input/";
    //"../meta/input/settings1.txt"
   // auto functions_trie = operations::get_functions_trie();

    std::string settings_file_path, program_file_path;
    std::unique_ptr<interpreter::builder<interpreter_rpn>> interpreter_builder;
    int current_index=0;



    if (argc==2 ||
        (argc>=2 &&
            (argv[2][0] == '-' || argv[2][0] == '/')))
    {
        std::ifstream input(last_settings_file_path);
        if (!(input >> settings_file_path))
        {
            throw std::invalid_argument("At least two argument was expected: settings and program file pathes");
        }

    }
    else if (argc < 3)
    {
        throw std::invalid_argument("At least two argument was expected: settings and program file pathes");
    }

    try
    {
        if (settings_file_path.empty())
        {
            settings_file_path = argv[++current_index];
            std::ofstream output(last_settings_file_path);
            output << settings_file_path;
        }
        program_file_path = argv[++current_index];
        interpreter_builder = std::make_unique<interpreter::builder<interpreter_rpn>>(settings_file_path);
        interpreter_builder->append_function_map(operations::get_functions_trie());
        interpreter_builder->set_variables_alphabet(interpreter::default_variables_alphabet);
    }
    catch (std::runtime_error const &e)
    {
        std::cout<<e.what();
        return -1;
    }

    try
    {
        std::string current_flag;
        for (int i=++current_index; i<argc;++i)
        {
            current_flag = argv[i];
            if (current_flag=="-base_a")
            {
                interpreter_builder->set_base_assign(std::atoi(argv[++i]));
            }
            else if (current_flag=="-base_i")
            {
                interpreter_builder->set_base_input(std::atoi(argv[++i]));
            }
            else if (current_flag=="-base_o")
            {
                interpreter_builder->set_base_output(std::atoi(argv[++i]));
            }
            else if (current_flag=="-enclosure")
            {
                interpreter_builder->set_comments_enclosure_max_level(std::atoi(argv[++i]));
            }
            else if (current_flag=="-debug" || argv[i]=="-d" || argv[i]=="/debug")
            {
                interpreter_builder->set_debug_mode(true);
            }
        }
    }
    catch (std::out_of_range&)
    {
        std::cout << "Incorrect arguments list";
        return -2;
    }

    std::unique_ptr<interpreter_rpn> interpr (interpreter_builder->build());
    interpreter_builder.reset();

    try
    {
        interpr ->run(program_file_path);
    }
    catch(const std::exception& e)
    {
        std::cout<<e.what()<<std::endl;
    }

    return 0;
}

