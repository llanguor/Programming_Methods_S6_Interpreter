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

    class iterator_base
    {

    private:

        comments_handler _comments_handler;
        comments_handler::control_char_iterator _comments_handler_it;
        std::variant<std::string, comments_handler::control_char_types> _current_value;
        int _position=0;
        std::regex const * _regex_separators;
        std::regex const * _regex_chars;

    public:

        explicit iterator_base(
           std::istream * stream,
           size_t enclosure_max_level,
           std::regex const * regex_chars,
           std::regex const * regex_separators,
           bool const & control_chars_need_to_handle);

    public:

        bool operator==(iterator_base const &other) const noexcept;

        std::variant<std::string, comments_handler::control_char_types> operator*() const;

        iterator_base &operator++();

        iterator_base operator++(int not_used);
    };

public:

    [[nodiscard]] iterator_base begin() const;

    [[nodiscard]] iterator_base end() const;

    [[nodiscard]] iterator_base begin_string_only() const;

    [[nodiscard]] iterator_base end_string_only() const;

};