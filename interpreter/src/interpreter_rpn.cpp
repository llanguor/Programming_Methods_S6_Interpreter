#include "interpreter_rpn.hpp"


#pragma region overrive calculate_expression

int interpreter_rpn::calculate_expression(std::string const &expression)
{
    auto rpn = expression_to_reverse_polish_notation(expression);
    auto result = execute_sequence_of_functions(rpn);
    return result;
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
    std::stack<expression_element> stack;
    std::queue<expression_element> queue;

    tokenizer tokenizer(input,_comments_enclosure_max_level,
                R"([(),])", R"([^(),])", false, true);

    for (auto it = tokenizer.begin_string_only(); it != tokenizer.end_string_only(); ++it)
    {
        char separator = it->right_separator;

        //count current function arguments
        if (separator==','
            || it->right_separator==')' && (it->left_separator==',' || it->left_separator==')'))
        {
            if (!stack.empty())
            {
                ++stack.top().arguments_count;
            }
        }


        //integer value
        if (it->right_separator==EOF && it->left_separator==EOF)
        {
            //move constructor call
            queue.push(expression_element(it->token));
        }
        //last bracket
        else if (it->token=="")
        {
            if (separator!=',')
            {
                stack.push(expression_element(std::string(1,separator)));
            }
        }
        //function
        else if (separator=='(')
        {
            stack.push(expression_element(std::string(1,separator)));
            stack.push(expression_element(it->token,0));
        }
        //variable or number
        else if (separator == ')' || separator == ',')
        {
            queue.push(expression_element(it->token));

            if (separator!=',')
            {
                stack.push(expression_element(std::string(1,separator)));
            }
        }


        if (!stack.empty() && stack.top().value==")")
        {
            while (!stack.empty() && stack.top().value==")")
            {
                stack.pop();
            }

            queue.push(std::move(stack.top()));
            stack.pop();
            stack.pop();
        }


    }

    return queue;
}

std::queue<interpreter_rpn::expression_element> interpreter_rpn::parse_to_rpn_before(std::string const &input)
{
    std::stack<std::string> stack;
    std::queue<expression_element> queue;
    int last_function_argc = 0;

    tokenizer tokenizer(input,_comments_enclosure_max_level,
                R"([(),])", R"([^(),])", false, true);

    for (auto it = tokenizer.begin_string_only(); it != tokenizer.end_string_only(); ++it)
    {
        char separator = it->left_separator;
      //  ((var_2,5)div,(var_1,2)rem)add
        if (separator==','
                    || separator==')' && (it->right_separator==',' || it->right_separator==')'))
        {
            if (!stack.empty())
            {
                ++last_function_argc;
            }
        }


        //integer value
        if (it->right_separator==EOF && it->left_separator==EOF)
        {
            queue.push(expression_element(it->token));
        }
        //last bracket
        else if (it->token=="")
        {
            if (separator!=',')
            {
                stack.push(std::string(1,separator));
            }
        }
        //function
        else if (separator ==')')
        {
            stack.push(it->token);
            stack.push(std::string(1,separator));
        }
        //variable or number
        else if (separator == '(' || separator == ',')
        {
            if (separator!=',')
            {
                stack.push(std::string(1,separator));
            }

            queue.push(expression_element(it->token));
        }


        if (!stack.empty() && stack.top()==")")
        {
            stack.pop();
            queue.push(expression_element(stack.top(), last_function_argc));
            last_function_argc=0;

            while (!stack.empty() && stack.top()!="(")
            {
                stack.pop();
            }
            stack.pop();
        }

    }

    return queue;
}

std::queue<interpreter_rpn::expression_element> interpreter_rpn::parse_to_rpn_around(std::string const &input)
{
    std::queue<expression_element> queue;
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
            std::cout<<"ARGC: "<<element.arguments_count<< std::endl;
            while (!stack.empty() && args.size()<element.arguments_count)
            {
                args.push_back(stack.top());
                stack.pop();
            }

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
