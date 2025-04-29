#ifndef COMMENTS_HANDLER_H
#define COMMENTS_HANDLER_H

#include <string>

class comments_handler
{
private:

    enum STREAM_TYPES
    {
        file,
        string
    };

private:

    STREAM_TYPES _stream_type;
    char const * _stream;
    int _enclosure_max_level;

public:

    comments_handler(
        char const * stream,
        int  enclosure_max_level);

    comments_handler(
        const std::string& stream,
       int  enclosure_max_level);

public:

    class iterator
    {
    private:

        comments_handler const * _owner;

        char const * _stream_ptr;

        int _multiline_comment_enclosure_level = 0;

        bool in_single_line_comment = false;

    public:

        explicit iterator(comments_handler const * owner);

    public:

        bool operator!=(iterator const &other) const noexcept;

        bool operator==(iterator const &other) const noexcept;

        char& operator*() const;

        iterator &operator++();

        iterator operator++(int not_used);

    };

public:

    iterator begin();

    iterator end();

};



#endif
