#include "tokenizer.hpp"

#pragma region tokenizer

tokenizer::tokenizer(
    std::istream * stream,
    size_t const & enclosure_max_level,
    std::string const & regex_separators,
    std::string const & regex_chars,
    bool const & emit_empty_tokens):
    _stream_ptr(stream),
    _enclosure_max_level(enclosure_max_level),
    _regex_chars(regex_chars),
    _regex_separators(regex_separators),
    _emit_empty_tokens(emit_empty_tokens)
{
    if (_stream_ptr->fail())
    {
        throw std::runtime_error("Stream operation failed: either the file could not be read or the data could not be extracted.");
    }
}

tokenizer::tokenizer(
    std::string const &str,
    size_t const &enclosure_max_level,
    std::string const &regex_separators,
    std::string const &regex_chars,
    bool const &is_file,
    bool const & emit_empty_tokens):
    _enclosure_max_level(enclosure_max_level),
    _regex_chars(regex_chars),
    _regex_separators(regex_separators),
    _emit_empty_tokens(emit_empty_tokens)
{
    if (is_file)
    {
        _stream = make_unique<std::ifstream>(str);
        if (!dynamic_cast<std::ifstream *>(&*_stream)->is_open())
        {
            throw std::runtime_error("Failed to open file: " + str +
                                ". File might not exist or you do not have permission to read it.");
        }
    }
    else
    {
        _stream = make_unique<std::stringstream>(str);
    }


    if (_stream->fail())
    {
        throw std::runtime_error("Stream operation failed: either the file could not be read or the data could not be extracted.");
    }

    _stream_ptr = &*_stream;
}


tokenizer::token_iterator tokenizer::begin() const
{
    return tokenizer::token_iterator(
        _stream_ptr,
        _enclosure_max_level,
        &_regex_chars,
        &_regex_separators,
        _emit_empty_tokens);
}

tokenizer::token_iterator tokenizer::end() const
{
    return tokenizer::token_iterator(
        nullptr,
        _enclosure_max_level,
        &_regex_chars,
        &_regex_separators,
        _emit_empty_tokens);
}


tokenizer::token_string_only_iterator tokenizer::begin_string_only() const
{
    return tokenizer::token_string_only_iterator(
    _stream_ptr,
    _enclosure_max_level,
    &_regex_chars,
    &_regex_separators,
    _emit_empty_tokens);
}

tokenizer::token_string_only_iterator tokenizer::end_string_only() const
{
    return tokenizer::token_string_only_iterator(
    nullptr,
    _enclosure_max_level,
    &_regex_chars,
    &_regex_separators,
    _emit_empty_tokens);
}

#pragma endregion

#pragma region token_iterator

tokenizer::token_iterator::token_iterator(
    std::istream * stream,
    size_t const enclosure_max_level,
    std::regex const * regex_chars,
    std::regex const * regex_separators,
    bool const & emit_empty_tokens):
    _comments_handler(stream, enclosure_max_level),
    _comments_handler_it(_comments_handler.begin()),
    _regex_separators(regex_separators),
    _regex_chars(regex_chars),
    _emit_empty_tokens(emit_empty_tokens)
{
    if (_comments_handler_it == _comments_handler.end())
    {
        _position = EOF;
    }
    else
    {
        ++(*this);
    }
}

bool tokenizer::token_iterator::operator==(token_iterator const &other) const noexcept
{
    return _position ==
        other._position;
}

tokenizer::iterator_data * tokenizer::token_iterator::operator*() const
{
    if (_position==EOF)
        throw std::out_of_range("Attempt to dereference end-iterator");

    return &*_current_data;
}

tokenizer::token_iterator & tokenizer::token_iterator::operator++()
{
    std::string token;
    ++_position;

    while (_comments_handler_it != _comments_handler.end())
    {
        auto current_char_value = *_comments_handler_it;
        ++_comments_handler_it;

        if (std::holds_alternative<comments_handler::control_char_types>(current_char_value))
        {
            _current_data =
                std::make_shared<iterator_data_control_char>(
                    std::get<comments_handler::control_char_types>(
                        current_char_value));
            return *this;
        }

        const char current_char = static_cast<char>(std::get<int>(current_char_value));
        std::string str(1, current_char);

        if (std::regex_match(str, *_regex_separators))
        {
            if (!token.empty() || _emit_empty_tokens)
            {
                _current_data = std::make_shared<iterator_data_string>(token, current_char, last_separator);
                last_separator = current_char;
                return *this;
            }
        }
        else if (std::regex_match(str, *_regex_chars))
        {
            token += current_char;
        }
        else
        {
          //  throw std::invalid_argument(std::format("Unexpected character {} on position {}", current_char, std::to_string(_position)));
        }
    }

    if (!token.empty())
    {
        _current_data = std::make_shared<iterator_data_string>(token, EOF, last_separator);
    }
    else
    {
        _position=EOF;
    }

    return *this;
}

tokenizer::token_iterator tokenizer::token_iterator::operator++(int not_used)
{
    token_iterator temp {*this};
    ++(*this);
    return temp;
}

#pragma endregion


#pragma region token_string_only_iterator

tokenizer::token_string_only_iterator::token_string_only_iterator(
    std::istream *stream,
    size_t enclosure_max_level,
    std::regex const *regex_chars,
    std::regex const *regex_separators,
    bool const & emit_empty_tokens):
    _it(stream, enclosure_max_level, regex_chars, regex_separators, emit_empty_tokens)
{
}

bool tokenizer::token_string_only_iterator::operator==(token_string_only_iterator const &other) const noexcept
{
    return _it.operator==(other._it);
}

tokenizer::iterator_data_string & tokenizer::token_string_only_iterator::operator*() const
{
    return *dynamic_cast<iterator_data_string *>(_it.operator*()); //std::get<std::string>(_it.operator*());
}

tokenizer::iterator_data_string * tokenizer::token_string_only_iterator::operator->() const
{
    return &**this;
}

tokenizer::token_string_only_iterator & tokenizer::token_string_only_iterator::operator++()
{
    try
    {
        while (dynamic_cast<iterator_data_control_char *>(*++_it))
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

tokenizer::token_string_only_iterator tokenizer::token_string_only_iterator::operator++(int not_used)
{
    token_string_only_iterator temp {*this};
    ++(*this);
    return temp;
}

#pragma endregion