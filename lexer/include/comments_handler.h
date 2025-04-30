#ifndef COMMENTS_HANDLER_H
#define COMMENTS_HANDLER_H

#include <string>
#include <fstream>
#include <variant>
#include <sstream>

class comments_handler
{

private:

    std::istream * _stream;
    int const _enclosure_max_level;

public:

    enum CONTROL_CHAR_TYPES
    {
        DEBUG
    };

public:

    comments_handler(
        std::istream * stream,
        int enclosure_max_level);

public:

    class iterator
    {
    private:

        std::istream * _stream;
        int _enclosure_max_level;

        std::variant<int, CONTROL_CHAR_TYPES> _current_value = EOF;
        int _multiline_comment_enclosure_level = 0;
        bool _in_single_line_comment = false;
        std::string _single_line_comment_cache;

    public:

        explicit iterator(
           std::istream * stream,
           int enclosure_max_level);

    public:

        bool operator==(iterator const &other) const noexcept;

        std::variant<int, CONTROL_CHAR_TYPES> operator*() const;

        iterator &operator++();

        iterator operator++(int not_used);

    };

public:

    [[nodiscard]] iterator begin() const;

    [[nodiscard]] iterator end() const;

};



#endif
