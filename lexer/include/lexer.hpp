#pragma once
#include <memory>
#include "comments_handler.hpp"
#include <regex>
#include <format>

class lexer
{
public:

    using control_char_types = comments_handler::control_char_types;

private:

    std::istream * _stream;
    int const _enclosure_max_level;
    std::regex const _regex_chars;
    std::regex const _regex_separators;

public:

    explicit lexer(
       std::istream * stream,
       int enclosure_max_level,
       std::string const & regex_chars,
       std::string const & regex_separators);

public:

    class iterator
    {

    private:

        comments_handler _comments_handler;
        comments_handler::iterator _comments_handler_it;
        std::variant<std::string, comments_handler::control_char_types> _current_value;
        int _position=0;
        std::regex const * _regex_chars;
        std::regex const * _regex_separators;

    public:

        explicit iterator(
           std::istream * stream,
           int enclosure_max_level,
           std::regex const * regex_chars,
           std::regex const * regex_separators);

    public:

        bool operator==(iterator const &other) const noexcept;

        std::variant<std::string, comments_handler::control_char_types> operator*() const;

        iterator &operator++();

        iterator operator++(int not_used);
    };

public:

    [[nodiscard]] iterator begin() const;

    [[nodiscard]] iterator end() const;

};