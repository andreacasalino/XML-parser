#include <ParserError.h>

namespace xmlPrs {
    ParserError::ParserError(const std::string& what)
        : std::runtime_error(what) {
    };
}