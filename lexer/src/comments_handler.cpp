#include "comments_handler.h"


comments_handler::comments_handler(char const *stream, int enclosure_max_level)
{

}

comments_handler::comments_handler(const std::string &stream, int enclosure_max_level)
{
}

comments_handler::iterator::iterator(
    comments_handler const *owner):
    _owner(owner), _stream_ptr(owner->_stream)
{
}

bool comments_handler::iterator::operator!=(iterator const &other) const noexcept
{
}

bool comments_handler::iterator::operator==(iterator const &other) const noexcept
{
}

char & comments_handler::iterator::operator*() const
{
}

comments_handler::iterator & comments_handler::iterator::operator++()
{
}

comments_handler::iterator comments_handler::iterator::operator++(int not_used)
{
}

comments_handler::iterator comments_handler::begin()
{
}

comments_handler::iterator comments_handler::end()
{
}
