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
    size_t const _enclosure_max_level;
    std::regex const _regex_chars;
    std::regex const _regex_separators;

public:

    explicit lexer(
       std::istream * stream,
       size_t const & enclosure_max_level,
       std::string const & regex_separators,
       std::string const & regex_chars);


public:

    class lexeme_iterator
    {

    private:

        comments_handler _comments_handler;
        comments_handler::control_char_iterator _comments_handler_it;
        std::variant<std::string, comments_handler::control_char_types> _current_value;
        int _position=0;
        std::regex const * _regex_separators;
        std::regex const * _regex_chars;

    public:

        explicit lexeme_iterator(
           std::istream * stream,
           size_t enclosure_max_level,
           std::regex const * regex_chars,
           std::regex const * regex_separators);

    public:

        bool operator==(lexeme_iterator const &other) const noexcept;

        std::variant<std::string, comments_handler::control_char_types> operator*() const;

        lexeme_iterator &operator++();

        lexeme_iterator operator++(int not_used);
    };


    class lexeme_string_only_iterator
    {

    private:

        lexeme_iterator _it;

    public:

        explicit lexeme_string_only_iterator(
           std::istream * stream,
           size_t enclosure_max_level,
           std::regex const * regex_chars,
           std::regex const * regex_separators);

        bool operator==(
            lexeme_string_only_iterator const &other) const noexcept;

        std::string operator*();

        lexeme_string_only_iterator
            & operator++();

        lexeme_string_only_iterator
            operator++(int not_used);
    };

public:

    [[nodiscard]] lexeme_iterator begin() const;

    [[nodiscard]] lexeme_iterator end() const;

    [[nodiscard]] lexeme_string_only_iterator begin_string_only() const;

    [[nodiscard]] lexeme_string_only_iterator end_string_only() const;

};