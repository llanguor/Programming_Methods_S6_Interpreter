#include "interpreter.hpp"
#include "debugger.hpp"

#pragma region static_fields

std::string const interpreter::default_variables_alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";

#pragma endregion

#pragma region constructors

interpreter::interpreter(

    trie<std::function<int(std::vector<int>)>> const &operations,
    lvalues_position_types const &lvalues_position,
    arguments_position_types const &arguments_position,
    size_t const & comments_enclosure_max_level,
    size_t const &base_assign,
    size_t const &base_input,
    size_t const &base_output,
    bool const &is_debug_mode_enabled,
    std::string const & variables_alphabet):

    _operations(operations),
    _lvalues_position(lvalues_position),
    _arguments_position(arguments_position),
    _comments_enclosure_max_level(comments_enclosure_max_level),
    _base_assign(base_assign),
    _base_input(base_input),
    _base_output(base_output),
    _is_debug_mode_enabled(is_debug_mode_enabled),
    _variables(variables_alphabet)
{
}

#pragma endregion

#pragma region setters

interpreter & interpreter::set_base_assign(size_t const &base)
{
    _base_assign = base;
    return *this;
}

interpreter & interpreter::set_base_input(size_t const &base)
{
    _base_input = base;
    return *this;
}

interpreter & interpreter::set_base_output(size_t const &base)
{
    _base_output = base;
    return *this;
}

interpreter & interpreter::set_debug_mode(bool const &is_debug_mode_enabled)
{
    _is_debug_mode_enabled = is_debug_mode_enabled;
    return *this;
}

#pragma endregion

#pragma region methods

void interpreter::run(std::string const &program_file_path, bool const &is_debug_mode_enabled)
{
    run(program_file_path, is_debug_mode_enabled, _base_assign, _base_input, _base_output);
}

void interpreter::run(std::string const &program_file_path)
{
    run(program_file_path, _is_debug_mode_enabled, _base_assign, _base_input, _base_output);
}

void interpreter::run(
    std::string const &program_file_path,
    bool const &is_debug_mode_enabled,
    size_t const &base_assign,
    size_t const &base_input,
    size_t const &base_output)
{
    std::cout<<" "<<std::endl;


    tokenizer _tokenizer_instruction(
        program_file_path,
        _comments_enclosure_max_level,
        R"(;)",
        _instructions_alphabet,
        true);

    int line_number = 0;
    for (auto const & value : _tokenizer_instruction)
    {
        if (std::holds_alternative<comments_handler::control_char_types>(value))
        {
            if (is_debug_mode_enabled)
            {
                switch (std::get<comments_handler::control_char_types>(value))
                {
                    case comments_handler::control_char_types::breakpoint:
                        auto result = debugger::handle_breakpoint(*this);
                        if (!result) return;
                        break;
                }
            }
            continue;
        }


        auto line = std::get<std::string>(value);
       // std::cout << "INSTRUCTION: [" <<line << "]" << std::endl;


        std::string variable_to_assign, expression_str;

        try
        {
            tokenizer _tokenizer_assignment(
            line,
            _comments_enclosure_max_level,
            R"(=)",
            _instructions_alphabet,
            false);

            auto _tokenizer_assignment_it = _tokenizer_assignment.begin_string_only();
            variable_to_assign = *_tokenizer_assignment_it;
            expression_str = *++_tokenizer_assignment_it;

            if (_lvalues_position == interpreter::right)
            {
                std::swap(variable_to_assign, expression_str);
            }
        }
        catch (std::out_of_range e)
        {
            throw std::runtime_error("Invalid content in program file on line " + std::to_string(line_number));
        }

        std::cout<<"VARIABLE: "<< variable_to_assign <<std::endl;
        std::cout<<"EXPRESSION: "<< expression_str <<std::endl;
        ++line_number;


        auto result = calculate_expression(expression_str);
        try
        {
            _variables.upsert(variable_to_assign, std::move(result));
        }
        catch (std::out_of_range e)
        {
            throw std::out_of_range(
                "Incorrect variable name at line " +
                std::to_string(line_number) +
                "character is not contained in alphabet");
        }
    }
}

#pragma endregion

#pragma region interpreter_rpn

int interpreter_rpn::calculate_expression(std::string const &expression)
{
    auto rpn = expression_to_reverse_polish_notation(expression);
    auto result = execute_sequence_of_functions(rpn);
    return result;
}

std::string const interpreter_rpn::expression_to_reverse_polish_notation(std::string const &input)
{
    return input;
}

int interpreter_rpn::execute_sequence_of_functions(std::string const &input)
{
    return -1;
}

#pragma endregion