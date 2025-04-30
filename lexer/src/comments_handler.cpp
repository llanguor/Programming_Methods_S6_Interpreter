#include "comments_handler.h"


#pragma region comments_handler

comments_handler::comments_handler(
    std::istringstream * stream,
    int const enclosure_max_level):
    _stream_type(STREAM_TYPES::STRING),
    _stream(stream),
    _enclosure_max_level(enclosure_max_level)
{
}

comments_handler::comments_handler(
    std::ifstream * stream,
    int const enclosure_max_level):
    _stream_type(STREAM_TYPES::FILE),
    _stream(stream),
    _enclosure_max_level(enclosure_max_level)
{
}

comments_handler::iterator comments_handler::begin()
{
    return comments_handler::iterator(this);
}

comments_handler::iterator comments_handler::end()
{
    return comments_handler::iterator(nullptr);
}

#pragma endregion

#pragma region iterator

comments_handler::iterator::iterator(
    comments_handler * owner):
    _owner(owner)
{
    if (owner!=nullptr)
    {
        owner->_stream->get(_current_char);
    }
}

bool comments_handler::iterator::operator==(iterator const &other) const noexcept
{
    return
        _owner==nullptr && other._owner==nullptr ||
        ( _owner!=nullptr && other._owner!=nullptr &&
            _owner->_stream->tellg() == other._owner->_stream->tellg());
}

int comments_handler::iterator::operator*() const
{
    if (_owner==nullptr)
        throw std::out_of_range("Attempt to dereference end-iterator");

    return _current_char;
}

comments_handler::iterator & comments_handler::iterator::operator++()
{
    if (_owner==nullptr)
        return *this;

    while (_owner->_stream->get(_current_char))
    {
        if (_current_char == '#'
            && _multiline_comment_enclosure_level == 0
            && !_in_single_line_comment)
        {
            _in_single_line_comment = true;
            continue;
        }

        if ((_current_char == '\n' || !_current_char) && _in_single_line_comment)
        {
            _in_single_line_comment = false;
            continue;
        }

        if (_current_char == '[' && !_in_single_line_comment)
        {
            if (++_multiline_comment_enclosure_level > _owner->_enclosure_max_level)
            {
                throw std::invalid_argument(
                    "Multiline comment enclosure is too high!");
            }

            continue;
        }

        if (_current_char == ']' && !_in_single_line_comment)
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

    if (_owner->_stream->eof())
    {
        _owner = nullptr;

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


