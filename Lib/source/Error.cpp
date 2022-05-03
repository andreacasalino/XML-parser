#include <XML-Parser/Error.h>

namespace xmlPrs {
Error::Error(const std::string &what) : std::runtime_error(what) {}
} // namespace xmlPrs