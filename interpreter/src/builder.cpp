#include "builder.hpp"

interpreter::builder::builder(
    std::string const & settings_file_path,
    int enclosure_max_level,
    std::regex const *regex_chars,
    std::regex const *regex_separators):
    _stream(settings_file_path),
    _lexer(&_stream, enclosure_max_level, regex_chars, regex_separators)
{
}
