#pragma once
#include <memory>
#include "comments_handler.hpp"
#include <regex>

class lexer
{
public:

    using CONTROL_CHAR_TYPES = comments_handler::CONTROL_CHAR_TYPES;

private:

    std::istream * _stream;
    int const _enclosure_max_level;
    std::regex const * _regex_chars;
    std::regex const * _separators_chars;

public:

    explicit lexer(
       std::istream * stream,
       int enclosure_max_level,
       std::regex const * regex_chars,
        std::regex const * separators_chars);

public:

    class iterator
    {

    private:

        comments_handler _comments_handler;
        comments_handler::iterator _comments_handler_it;
        std::variant<std::string, comments_handler::CONTROL_CHAR_TYPES> _current_value;
        int _position=0;
        std::regex const * _regex_chars;
        std::regex const * _separators_chars;

    public:

        explicit iterator(
           std::istream * stream,
           int enclosure_max_level,
           std::regex const * regex_chars,
           std::regex const * separators_chars);

    public:

        bool operator==(iterator const &other) const noexcept;

        std::variant<std::string, comments_handler::CONTROL_CHAR_TYPES> operator*() const;

        iterator &operator++();

        iterator operator++(int not_used);
    };

public:

    [[nodiscard]] iterator begin() const;

    [[nodiscard]] iterator end() const;

};