#pragma once
#include <string>
#include <queue>
#include <utility>
#include "interpreter.hpp"

class interpreter_rpn final : public interpreter
{
private:

    enum expression_element_type
    {
        operand,
        function
    };

    class expression_element final
    {
    public:

        expression_element_type type;
        std::string const value;
        int arguments_count;

        explicit expression_element(
            expression_element_type const & type,
            std::string  value):
            type(type),
            value(std::move(value)),
            arguments_count(0)
        {
        }

        explicit expression_element(
            expression_element_type const & type,
            std::string value,
            int const & arguments_count):
            type(type),
            value(std::move(value)),
            arguments_count(arguments_count)
        {
        }
    };

public:

    using interpreter::interpreter;

private:

    int calculate_expression(
        std::string const & expression,
        int const & line_number) override;

private:

    std::queue<interpreter_rpn::expression_element>
    expression_to_reverse_polish_notation(
        std::string const &input) const;

    int execute_sequence_of_functions(
        std::queue<interpreter_rpn::expression_element> & input_queue);

private:

    std::queue<interpreter_rpn::expression_element> parse_to_rpn_after(
        std::string const & input) const;

    std::queue<interpreter_rpn::expression_element> parse_to_rpn_before(
        std::string const & input) const;

    std::queue<interpreter_rpn::expression_element> parse_to_rpn_around(
        std::string const & input) const;

    int entry_to_value(
      std::string const & variable);

};