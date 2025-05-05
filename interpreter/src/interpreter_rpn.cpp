#include "interpreter_rpn.hpp"


#pragma region overrive calculate_expression

int interpreter_rpn::calculate_expression(
    std::string const &expression,
    int const & line_number)
{
    try
    {
        auto rpn = expression_to_reverse_polish_notation(expression);
        return execute_sequence_of_functions(rpn);
    }
    catch (std::runtime_error e)
    {
        throw std::runtime_error(
            "Error at line "+
            std::to_string(line_number) +
            ": "+
            e.what());
    }
}

#pragma endregion

#pragma region functions

std::queue<interpreter_rpn::expression_element> const
    interpreter_rpn::expression_to_reverse_polish_notation(std::string const &input)
{
    std::queue<interpreter_rpn::expression_element> queue;
    switch (_arguments_position)
    {
        case interpreter::after_operation:
            return parse_to_rpn_after(input);
            break;

        case interpreter::before_operation:
            return parse_to_rpn_before(input);
            break;

        case interpreter::around_operation:
            return parse_to_rpn_around(input);
            break;
    }

    return queue;
}


#pragma endregion

#pragma region parser functions

std::queue<interpreter_rpn::expression_element>
    interpreter_rpn::parse_to_rpn_after(std::string const &input)
{
    std::stack<std::string> stack;
    std::queue<expression_element> queue;
    std::stack<int> arguments_count_stack;
    tokenizer tokenizer(input,_comments_enclosure_max_level,
                R"([(),])", R"([^(),])", false, true);

    for (auto it = tokenizer.begin_string_only(); it != tokenizer.end_string_only(); ++it)
    {
        //integer value
        if (it->right_separator==EOF && it->left_separator==EOF)
        {
            queue.push(expression_element(operand, it->token));
        }
        //last bracket
        else if (it->token=="")
        {
            if (it->right_separator!=',')
            {
                stack.push(std::string(1,it->right_separator));
            }
        }
        //function
        else if (it->right_separator=='(')
        {
            stack.push(std::string(1,it->right_separator));
            stack.push(it->token);
            arguments_count_stack.push(0);
        }
        //variable or number
        else if (it->right_separator == ')' || it->right_separator == ',')
        {
            if (arguments_count_stack.empty())
            {
                throw std::runtime_error("incorrect syntax");
            }
            ++arguments_count_stack.top();

            queue.push(expression_element(operand, it->token));
            if (it->right_separator!=',')
            {
                stack.push(std::string(1,it->right_separator));
            }
        }


        if (!stack.empty() && stack.top()==")")
        {
            while (!stack.empty() && stack.top()==")")
            {
                stack.pop();
            }

            if (stack.size()<2 || arguments_count_stack.empty())
            {
                throw std::runtime_error("incorrect syntax");
            }

            queue.push(expression_element(
                function,
                std::move(stack.top()), ////!
                std::move(arguments_count_stack.top())
                ));

            stack.pop();
            stack.pop();
            arguments_count_stack.pop();
            if (!arguments_count_stack.empty())
            {
                ++arguments_count_stack.top();
            }
        }


    }

    return queue;
}

std::queue<interpreter_rpn::expression_element> interpreter_rpn::parse_to_rpn_before(std::string const &input)
{
    std::stack<std::string> stack;
    std::queue<expression_element> queue;
    std::stack<int> arguments_count_stack;
    tokenizer tokenizer(input,_comments_enclosure_max_level,
                R"([(),])", R"([^(),])", false, true);

    for (auto it = tokenizer.begin_string_only(); it != tokenizer.end_string_only(); ++it)
    {
        //integer value
        if (it->right_separator==EOF && it->left_separator==EOF)
        {
            queue.push(expression_element(operand, it->token));
        }
        //last bracket
        else if (it->token=="")
        {
            if (it->left_separator!=',')
            {
                stack.push(std::string(1,it->left_separator));
                arguments_count_stack.push(0);
            }
        }
        //function
        else if (it->left_separator ==')')
        {
            stack.push(it->token);
            stack.push(std::string(1,it->left_separator));
        }
        //variable or number
        else if (it->left_separator == '(' || it->left_separator == ',')
        {
            if (it->left_separator!=',')
            {
                stack.push(std::string(1,it->left_separator));
                arguments_count_stack.push(0);
            }

            queue.push(expression_element(operand, it->token));
            if (arguments_count_stack.empty())
            {
                throw std::runtime_error("incorrect syntax");
            }
            ++arguments_count_stack.top();
        }


        if (!stack.empty() && stack.top()==")")
        {
            while (!stack.empty() && stack.top()==")")
            {
                stack.pop();
            }

            if (stack.empty() || arguments_count_stack.empty())
            {
                throw std::runtime_error("incorrect syntax");
            }

            queue.push(expression_element(
               function,
               std::move(stack.top()), ////!
               std::move(arguments_count_stack.top())
               ));

            stack.pop();
            arguments_count_stack.pop();
            if (!arguments_count_stack.empty())
            {
                ++arguments_count_stack.top();
            }
        }

    }

    return queue;
}

std::queue<interpreter_rpn::expression_element> interpreter_rpn::parse_to_rpn_around(std::string const &input)
{
    bool last_token_is_operator = false;
    std::stack<std::string> stack;
    std::queue<expression_element> queue;
    std::stack<int> arguments_count_stack;
    tokenizer tokenizer(input,_comments_enclosure_max_level,
                    R"([(),])", R"([^(),])", false, true);


    for (auto it = tokenizer.begin_string_only(); it != tokenizer.end_string_only(); ++it)
    {
        //first or last bracket
        if (it->token!="" && it->right_separator==EOF && it->left_separator==EOF)
        {
            queue.push(expression_element(operand, it->token));
        }
        else if (it->token=="")
        {
            if (it->left_separator!=',' && it->right_separator!=',')
            {
                stack.push(std::string(1,it->left_separator));
            }
        }
        //left variable
        else if (it->left_separator=='(' && it->right_separator==',')
        {
            stack.push(std::string(1,it->left_separator));
            queue.push(expression_element(operand, it->token));
            last_token_is_operator=true;
        }
        //function 2 arg
        else if (it->left_separator==',' && it->right_separator==','
            || it->left_separator==')' && it->right_separator=='(')
        {
            stack.push(it->token);
            arguments_count_stack.push(2);
            last_token_is_operator=false;
        }
        //function 1 arg
        else if (last_token_is_operator==true &&
            it->left_separator==',' &&
            (it->right_separator==')' || it->right_separator==EOF))
        {
            stack.push(it->token);
            stack.push(std::string(1,it->right_separator));
            arguments_count_stack.push(1);
            last_token_is_operator=false;

        }
        //right variable
        else if (it->left_separator==',' && it->right_separator==')')
        {
            stack.push(std::string(1,it->right_separator));
            queue.push(expression_element(operand, it->token));
            last_token_is_operator=true;
        }

        if (!stack.empty() && stack.top()==")")
        {
            while (!stack.empty() && stack.top()==")")
            {
                stack.pop();
            }

            if (stack.size()<2 || arguments_count_stack.empty())
            {
                throw std::runtime_error("incorrect syntax");
            }

            queue.push(expression_element(
                function,
                std::move(stack.top()), ////!
                std::move(arguments_count_stack.top())
                ));

            stack.pop();
            stack.pop();
            arguments_count_stack.pop();
        }

    }


    return queue;
}

int interpreter_rpn::entry_to_value(std::string const &variable)
{
    int value;
    try
    {
        value = _variables.obtain(variable);
    }
    catch(std::out_of_range const &e)
    {
        value = operations::base_to_decimal(variable, _base_assign);
    }

    return value;

}

#pragma endregion

#pragma region execute functions

int interpreter_rpn::execute_sequence_of_functions(
    std::queue<interpreter_rpn::expression_element> &input_queue)
{

    std::stack<int> stack;

    while (!input_queue.empty())
    {
        auto element = std::move(input_queue.front());
        input_queue.pop();

        if (element.type == interpreter_rpn::operand)
        {
            stack.push(entry_to_value(element.value));
        }
        else if (element.type == interpreter_rpn::function)
        {
            std::vector<int> args;

            while (!stack.empty() && args.size()<element.arguments_count)
            {
                args.push_back(stack.top());
                stack.pop();
            }
            std::ranges::reverse(args);

            try
            {
                auto func = _operations.obtain(element.value);
                stack.push(func(args));
            }
            catch (std::out_of_range const &e)
            {
                throw std::runtime_error("Function not found in the function list: " + element.value);
            }
            catch (std::invalid_argument const &e)
            {
                throw std::runtime_error("Error occurred while executing the function "
                    + element.value
                    +": "
                    +e.what());
            }

        }
    }

    return stack.top();
}

#pragma endregion
