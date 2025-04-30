#ifndef COMMENTS_HANDLER_H
#define COMMENTS_HANDLER_H

#include <string>
#include <sstream>
#include <memory>
#include <fstream>

class comments_handler
{
private:

    enum STREAM_TYPES
    {
        FILE,
        STRING
    };

private:

    std::istream *_stream;
    int _enclosure_max_level;
    STREAM_TYPES _stream_type;

public:

    //For string
    comments_handler(
        std::istringstream * stream,
        int enclosure_max_level);

    //For file
    comments_handler(
        std::ifstream * stream,
       int enclosure_max_level);

public:

    class iterator
    {
    private:

        comments_handler * _owner;
        int _multiline_comment_enclosure_level = 0;
        bool _in_single_line_comment = false;
        char _current_char = EOF;

    public:

        explicit iterator(
            comments_handler * owner);

    public:

        bool operator==(iterator const &other) const noexcept;

        int operator*() const;

        iterator &operator++();

        iterator operator++(int not_used);

    };

public:

    iterator begin();

    iterator end();

};



#endif
