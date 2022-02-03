#include <Error.h>

namespace xmlPrs {
Error::Error(const std::string &what) : std::runtime_error(what){};
}