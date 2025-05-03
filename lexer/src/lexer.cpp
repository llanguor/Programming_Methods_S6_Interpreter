#include "lexer.hpp"

#include <iostream>
#include <optional>
#include <utility>

#pragma region lexer

lexer::lexer(
    std::istream * stream,
    size_t const & enclosure_max_level,
    std::string const & regex_separators,
    std::string const & regex_chars):
    _stream(stream),
    _enclosure_max_level(enclosure_max_level),
    _regex_chars(regex_chars),
    _regex_separators(regex_separators)
{
}

lexer::lexeme_iterator lexer::begin() const
{
    return lexer::lexeme_iterator(
        _stream,
        _enclosure_max_level,
        &_regex_chars,
        &_regex_separators);
}

lexer::lexeme_iterator lexer::end() const
{
    return lexer::lexeme_iterator(
        nullptr,
        _enclosure_max_level,
        &_regex_chars,
        &_regex_separators);
}

lexer::lexeme_string_only_iterator lexer::begin_string_only() const
{
    return lexer::lexeme_string_only_iterator(
    _stream,
    _enclosure_max_level,
    &_regex_chars,
    &_regex_separators);
}

lexer::lexeme_string_only_iterator lexer::end_string_only() const
{
    return lexer::lexeme_string_only_iterator(
    nullptr,
    _enclosure_max_level,
    &_regex_chars,
    &_regex_separators);
}

#pragma endregion

#pragma region lexeme_iterator


lexer::lexeme_iterator::lexeme_iterator(
    std::istream * stream,
    size_t const enclosure_max_level,
    std::regex const * regex_chars,
    std::regex const * regex_separators):
    _comments_handler(stream, enclosure_max_level),
    _comments_handler_it(_comments_handler.begin()),
    _regex_separators(regex_separators),
    _regex_chars(regex_chars)
{
    if (_comments_handler_it == _comments_handler.end())
    {
        _position = EOF;
    }
    else
    {
        ++(*this);
    }
}

bool lexer::lexeme_iterator::operator==(lexeme_iterator const &other) const noexcept
{
    return _position ==
        other._position;
}

std::variant<std::string, comments_handler::control_char_types> lexer::lexeme_iterator::operator*() const
{
    if (_position==EOF)
        throw std::out_of_range("Attempt to dereference end-iterator");

    return _current_value;
}

lexer::lexeme_iterator & lexer::lexeme_iterator::operator++()
{
    std::string lexeme;
    ++_position;

    while (_comments_handler_it != _comments_handler.end())
    {
        auto current_char_value = *_comments_handler_it;
        ++_comments_handler_it;

        if (std::holds_alternative<comments_handler::control_char_types>(current_char_value))
        {
            _current_value = std::get<comments_handler::control_char_types>(current_char_value);
            return *this;
        }

        const char current_char = static_cast<char>(std::get<int>(current_char_value));
        std::string str(1, current_char);

        if (std::regex_match(str, *_regex_chars))
        {
            lexeme += current_char;
        }
        else if (std::regex_match(str, *_regex_separators))
        {
            if (!lexeme.empty())
            {
                _current_value = lexeme;
                return *this;
            }
        }
        else
        {
          //  throw std::invalid_argument(std::format("Unexpected character {} on position {}", current_char, std::to_string(_position)));
        }
    }

    if (!lexeme.empty())
    {
        _current_value = lexeme;
    }
    else
    {
        _position=EOF;
    }

    return *this;
}

lexer::lexeme_iterator lexer::lexeme_iterator::operator++(int not_used)
{
    lexeme_iterator temp {*this};
    ++(*this);
    return temp;
}

#pragma endregion

#pragma region lexeme_string_only_iterator

lexer::lexeme_string_only_iterator::lexeme_string_only_iterator(
    std::istream *stream,
    size_t enclosure_max_level,
    std::regex const *regex_chars,
    std::regex const *regex_separators):
    _it(stream, enclosure_max_level, regex_chars, regex_separators)
{
}

bool lexer::lexeme_string_only_iterator::operator==(lexeme_string_only_iterator const &other) const noexcept
{
    return _it.operator==(other._it);
}

std::string lexer::lexeme_string_only_iterator::operator*()
{
    return std::get<std::string>(_it.operator*());
}

lexer::lexeme_string_only_iterator & lexer::lexeme_string_only_iterator::operator++()
{
    try
    {
        while (
            std::holds_alternative<comments_handler::control_char_types>(
                *++_it))
        {

        }
    }
    catch (std::out_of_range e)
    {
        //here char_iterator end
        //or create class field "it_end" and init from constructor
        //it's bad: code dependency
    }

    return *this;
}

lexer::lexeme_string_only_iterator lexer::lexeme_string_only_iterator::operator++(int not_used)
{
    lexeme_string_only_iterator temp {*this};
    ++(*this);
    return temp;
}

#pragma endregion