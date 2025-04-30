#include "comments_handler.hpp"

#include <iostream>

#pragma region comments_handler

comments_handler::comments_handler(
    std::istream * stream,
    int const enclosure_max_level):
    _stream(stream),
    _enclosure_max_level(enclosure_max_level)
{
}

comments_handler::iterator comments_handler::begin() const
{
    return comments_handler::iterator(_stream, _enclosure_max_level);
}

comments_handler::iterator comments_handler::end() const
{
    return comments_handler::iterator(nullptr, _enclosure_max_level);
}

#pragma endregion

#pragma region iterator

comments_handler::iterator::iterator(
    std::istream * stream,
    int const enclosure_max_level):
    _stream(stream),
    _enclosure_max_level(enclosure_max_level)
{
    if (!_stream || !*_stream)
    {
       _stream = nullptr;
    }
    else
    {
        _current_value = _stream->get();
    }
}

bool comments_handler::iterator::operator==(iterator const &other) const noexcept
{
    return
        _stream==nullptr && other._stream==nullptr ||
        ( _stream!=nullptr && other._stream!=nullptr &&
            _stream->tellg() == other._stream->tellg());
}

std::variant<int, comments_handler::control_char_types> comments_handler::iterator::operator*() const
{
    if (_stream==nullptr)
        throw std::out_of_range("Attempt to dereference end-iterator");

    return _current_value;
}

comments_handler::iterator & comments_handler::iterator::operator++()
{
    if (_stream==nullptr)
        return *this;

    char ch;
    while (_stream->get(ch))
    {
        _current_value = ch;

        if (_in_single_line_comment)
        {
            _single_line_comment_cache+=ch;

            if (_single_line_comment_cache == "DEBUG")
            {
                _current_value = comments_handler::control_char_types::debug;
                return *this;
            }
        }

        if (ch == '#'
            && _multiline_comment_enclosure_level == 0
            && !_in_single_line_comment)
        {
            _in_single_line_comment = true;
            continue;
        }

        if ((ch == '\n' || !ch) && _in_single_line_comment)
        {
            _in_single_line_comment = false;
            _single_line_comment_cache="";
            continue;
        }

        if (ch == '[' && !_in_single_line_comment)
        {
            if (++_multiline_comment_enclosure_level > _enclosure_max_level)
            {
                throw std::invalid_argument(
                    "Multiline comment enclosure is too high!");
            }

            continue;
        }

        if (ch == ']' && !_in_single_line_comment)
        {
            if (--_multiline_comment_enclosure_level == -1)
            {
                throw std::invalid_argument(
                    "Attempt to break multiline comment, which hasn't been opened");
            }
            continue;
        }

        if (!_in_single_line_comment && _multiline_comment_enclosure_level == 0)
        {
            return *this;
        }
    }

    if (_stream->eof())
    {
        _stream = nullptr;

        if (_in_single_line_comment)
            _in_single_line_comment = false;

        if (_multiline_comment_enclosure_level != 0)
        {
            throw std::invalid_argument(
                "Multiline comment, which has been opened before, hasn't been broken");
        }
    }

    return *this;
}

comments_handler::iterator comments_handler::iterator::operator++(int not_used)
{
    iterator temp {*this};
    ++(*this);
    return temp;
}

#pragma endregion


