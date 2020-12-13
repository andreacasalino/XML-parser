#include <ParserError.h>
#include "ErrorHandler.h"

namespace xmlPrs {
    ParserError::ParserError(const std::string& what)
        : std::runtime_error(what) {
    };

    void UseThrowError() {
        ErrorHandler::useThrow();
    };

    void UseInformativeCout() {
        ErrorHandler::useCout();
    };
}