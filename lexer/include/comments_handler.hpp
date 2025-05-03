#pragma once
#include <string>
#include <fstream>
#include <variant>
#include <sstream>

class comments_handler
{

private:

    std::istream * _stream;
    size_t const _enclosure_max_level;

public:

    enum control_char_types
    {
        debug
    };

public:

    comments_handler(
        std::istream * stream,
        size_t const & enclosure_max_level);

public:

    class iterator
    {
    private:

        std::istream * _stream;
        int _enclosure_max_level;

        std::variant<int, control_char_types> _current_value = EOF;
        size_t _multiline_comment_enclosure_level = 0;
        bool _in_single_line_comment = false;
        std::string _single_line_comment_cache;

    public:

        explicit iterator(
           std::istream * stream,
           size_t const & enclosure_max_level);

    public:

        bool operator==(iterator const &other) const noexcept;

        std::variant<int, control_char_types> operator*() const;

        iterator &operator++();

        iterator operator++(int not_used);

    };

public:

    [[nodiscard]] iterator begin() const;

    [[nodiscard]] iterator end() const;

};