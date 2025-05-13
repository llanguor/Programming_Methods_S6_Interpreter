#include "comments_handler.hpp"
#include <iostream>


#pragma region static_fields

std::map<std::string,  comments_handler::control_char_types> const comments_handler::control_char_types_map =
{
    { "BREAKPOINT", comments_handler::control_char_types::breakpoint },
    { "breakpoint", comments_handler::control_char_types::breakpoint }
};

#pragma endregion

#pragma region comments_handler

comments_handler::comments_handler(
    std::istream * stream,
    size_t const & enclosure_max_level):
    _stream(stream),
    _enclosure_max_level(enclosure_max_level)
{
}

comments_handler::control_char_iterator comments_handler::begin() const
{
    return comments_handler::control_char_iterator(_stream, _enclosure_max_level);
}

comments_handler::control_char_iterator comments_handler::end() const
{
    return comments_handler::control_char_iterator(nullptr, _enclosure_max_level);
}

comments_handler::char_iterator comments_handler::begin_char_only() const
{
    return comments_handler::char_iterator(_stream, _enclosure_max_level);
}

comments_handler::char_iterator comments_handler::end_char_only() const
{
    return comments_handler::char_iterator(nullptr, _enclosure_max_level);
}


#pragma endregion

#pragma region control_char_iterator

comments_handler::control_char_iterator::control_char_iterator(
    std::istream * stream,
    size_t const & enclosure_max_level):
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

bool comments_handler::control_char_iterator::operator==(control_char_iterator const &other) const noexcept
{
    return
        _stream==nullptr && other._stream==nullptr ||
        ( _stream!=nullptr && other._stream!=nullptr &&
            _stream->tellg() == other._stream->tellg());
}

std::variant<int, comments_handler::control_char_types> const & comments_handler::control_char_iterator::operator*()
{
    if (_stream==nullptr)
        throw std::out_of_range("Attempt to dereference end-iterator");

    return _current_value;
}

comments_handler::control_char_iterator & comments_handler::control_char_iterator::operator++()
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

            if (control_char_types_map.contains(
                _single_line_comment_cache))
            {
                _current_value = control_char_types_map.at(_single_line_comment_cache);
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

comments_handler::control_char_iterator comments_handler::control_char_iterator::operator++(int not_used)
{
    control_char_iterator temp {*this};
    ++(*this);
    return temp;
}

#pragma endregion

#pragma region char_iterator

comments_handler::char_iterator::char_iterator(
    std::istream *stream,
    size_t const &enclosure_max_level):
    _it(stream, enclosure_max_level)
{
}

bool comments_handler::char_iterator::operator==(
    char_iterator const &other) const noexcept
{
    return _it.operator==(other._it);
}

int comments_handler::char_iterator::operator*()
{
    return std::get<int>(_it.operator*());
}

comments_handler::char_iterator
    & comments_handler::char_iterator::operator++()
{

    try
    {
        while (
            std::holds_alternative<comments_handler::control_char_types>(
                *++_it))
        {

        }
    }
    catch (std::out_of_range e)
    {
        //here char_iterator end
        //or create class field "it_end" and init from constructor
        //it's bad: code dependency
    }

    return *this;
}

comments_handler::char_iterator
    comments_handler::char_iterator::operator++(int not_used)
{
    char_iterator temp {*this};
    ++(*this);
    return temp;
}


#pragma endregion

