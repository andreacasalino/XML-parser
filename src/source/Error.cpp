/**
 * Author:    Andrea Casalino
 * Created:   04.09.2024
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <XML-Parser/Error.h>

namespace xmlPrs {
Error::Error(const std::string &what) : std::runtime_error(what) {}
} // namespace xmlPrs