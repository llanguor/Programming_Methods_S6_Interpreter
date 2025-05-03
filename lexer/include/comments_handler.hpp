#pragma once
#include <string>
#include <fstream>
#include <map>
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

    static std::map<std::string, control_char_types> const control_char_types_map;

public:

    comments_handler(
        std::istream * stream,
        size_t const & enclosure_max_level);

public:

    class control_char_iterator
    {
    private:

        std::istream * _stream;
        int const _enclosure_max_level;

        std::variant<int, control_char_types> _current_value = EOF;
        size_t _multiline_comment_enclosure_level = 0;
        bool _in_single_line_comment = false;
        std::string _single_line_comment_cache;

    public:

        explicit control_char_iterator(
           std::istream * stream,
           size_t const & enclosure_max_level);

    public:

        bool operator==(control_char_iterator const &other) const noexcept;

        virtual std::variant<int, control_char_types> operator*() const;

        control_char_iterator &operator++();

        control_char_iterator operator++(int not_used);

        virtual ~control_char_iterator() = default;
    };


    class char_iterator
    {
    private:

        control_char_iterator _it;

    public:

        char_iterator(
            std::istream *stream,
            size_t const &enclosure_max_level);

        bool operator==(
            char_iterator const &other) const noexcept;

        int operator*();

        char_iterator
            & operator++();

        char_iterator
            operator++(int not_used);
    };

public:

    [[nodiscard]] control_char_iterator begin() const;

    [[nodiscard]] control_char_iterator end() const;

    [[nodiscard]] char_iterator begin_char_only() const;

    [[nodiscard]] char_iterator end_char_only() const;
};