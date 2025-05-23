#include "interpreter.hpp"
#include "debugger.hpp"


#pragma region static_fields

std::string const interpreter::default_variables_alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";

const std::string interpreter::system_variables::base_input = "base_input";
const std::string interpreter::system_variables::base_output = "base_output";
const std::string interpreter::system_variables::base_assign = "base_assign";

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
    _is_debug_mode_enabled(is_debug_mode_enabled),
    _base_assign(base_assign),
    _base_input(base_input),
    _base_output(base_output),
    _variables(variables_alphabet)
{
    try
    {
        _variables.upsert(system_variables::base_input, base_input);
        _variables.upsert(system_variables::base_output, base_output);
        _variables.upsert(system_variables::base_assign, base_assign);
    }
    catch (std::out_of_range&)
    {
        throw std::invalid_argument("the variable alphabet must include symbols for the interpreter's reserved words: base_input, base_output, base_assign");
    }
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

void interpreter::run(std::string const &program_file_path)
{
    run(program_file_path, _is_debug_mode_enabled);
}

void interpreter::run(
    std::string const &program_file_path,
    bool const &is_debug_mode_enabled)
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
        if (auto control_char_value = dynamic_cast<tokenizer::iterator_data_control_char *>(value))
        {
            if (is_debug_mode_enabled)
            {
                switch (control_char_value->value)
                {
                    case comments_handler::control_char_types::breakpoint:
                        if (!debugger::handle_breakpoint(*this)) return;
                        break;
                }
            }
            continue;
        }


        auto line = dynamic_cast<tokenizer::iterator_data_string *>(value)->token;
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
            variable_to_assign = _tokenizer_assignment_it->token;

            ++_tokenizer_assignment_it;
            if (_tokenizer_assignment_it != _tokenizer_assignment.end_string_only())
            {
                expression_str = _tokenizer_assignment_it->token;
            }

            if (_lvalues_position == interpreter::right)
            {
                std::swap(variable_to_assign, expression_str);
            }

            if (expression_str.empty())
            {
                expression_str = variable_to_assign;
                variable_to_assign.clear();
            }
        }
        catch (std::out_of_range e)
        {
            throw std::runtime_error("Invalid content in program file on line " + std::to_string(line_number));
        }

        if (!variable_to_assign.empty() && std::isdigit(variable_to_assign[0]))
        {
            throw std::runtime_error(std::format("Incorrect variable name on line {}: first char can't be number", line_number));
        }


        auto result = calculate_expression(expression_str, line_number);

        try
        {
            if (!variable_to_assign.empty() && variable_to_assign!="_")
            {
                _variables.upsert(variable_to_assign, std::forward<int>(result));
            }
        }
        catch (std::out_of_range&)
        {
            throw std::out_of_range(
                "Incorrect variable name at line " +
                std::to_string(line_number) +
                "character is not contained in alphabet");
        }

        ++line_number;

    }
}

#pragma endregion