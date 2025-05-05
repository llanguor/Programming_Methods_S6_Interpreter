#pragma once
#include <memory>
#include "comments_handler.hpp"
#include <regex>
#include <format>
#include <iostream>
#include <optional>
#include <utility>

class tokenizer
{
public:

    using control_char_types = comments_handler::control_char_types;

private:

    std::unique_ptr<std::istream> _stream;
    std::istream * _stream_ptr;
    size_t const _enclosure_max_level;
    std::regex const _regex_chars;
    std::regex const _regex_separators;
    bool _emit_empty_tokens;

public:

    tokenizer(
       std::istream * stream,
       size_t const & enclosure_max_level,
       std::string const & regex_separators,
       std::string const & regex_chars,
       bool const & emit_empty_tokens=false);

    tokenizer(
       std::string const &str,
       size_t const & enclosure_max_level,
       std::string const & regex_separators,
       std::string const & regex_chars,
       bool const & is_file,
       bool const & emit_empty_tokens=false
       );


public:

    struct iterator_data
    {
        virtual ~iterator_data() = default;
    };

    struct iterator_data_string final : iterator_data
    {
    public:

        std::string const token;

        char const right_separator;

        char const left_separator;

    public:

        explicit iterator_data_string(
            std::string const & token,
            char const & right_separator,
            char const & left_separator):
            token(token),
            right_separator(right_separator),
            left_separator(left_separator)
        {
        };

    };

    struct iterator_data_control_char final : iterator_data
    {

    public:

        control_char_types const value;

    public:

        explicit iterator_data_control_char(
            control_char_types const & value):
        value(value) {};

    };

public:

    class token_iterator
    {

    private:

        comments_handler _comments_handler;
        comments_handler::control_char_iterator _comments_handler_it;
        std::variant<std::string, comments_handler::control_char_types> _current_value;
        int _position=0;
        std::regex const * _regex_separators;
        std::regex const * _regex_chars;
        std::shared_ptr<iterator_data> _current_data;
        bool _emit_empty_tokens;
        char last_separator = EOF;

    public:

        explicit token_iterator(
           std::istream * stream,
           size_t enclosure_max_level,
           std::regex const * regex_chars,
           std::regex const * regex_separators,
           bool const & emit_empty_tokens);

    public:

        bool operator==(token_iterator const &other) const noexcept;

        iterator_data * operator*() const;

        token_iterator &operator++();

        token_iterator operator++(int not_used);
    };

    class token_string_only_iterator
    {

    private:

        token_iterator _it;

    public:

        explicit token_string_only_iterator(
           std::istream * stream,
           size_t enclosure_max_level,
           std::regex const * regex_chars,
           std::regex const * regex_separators,
           bool const & emit_empty_tokens);

        bool operator==(
            token_string_only_iterator const &other) const noexcept;

        iterator_data_string & operator*() const;

        iterator_data_string* operator->() const;

        token_string_only_iterator
            & operator++();

        token_string_only_iterator
            operator++(int not_used);
    };

public:

    [[nodiscard]] token_iterator begin() const;

    [[nodiscard]] token_iterator end() const;

    [[nodiscard]] token_string_only_iterator begin_string_only() const;

    [[nodiscard]] token_string_only_iterator end_string_only() const;


};