#include "builder.hpp"

interpreter::builder::builder(
    std::string const & settings_file_path,
    int const enclosure_max_level):
    _stream(settings_file_path),
    _lexer(&_stream, enclosure_max_level, "[a-zA-Z0-9_]", "[\r\n\t ]+")
{
    for (auto l : _lexer)
    {
        if (std::holds_alternative<lexer::control_char_types>(l))
        {
            throw std::invalid_argument("the use of control codes is not supported in the context of a settings file");
        }

        std::cout << std::get<std::string>(l) <<" ";

    }
}
