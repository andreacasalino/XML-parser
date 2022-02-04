/**
 * Author:    Andrea Casalino
 * Created:   03.12.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#include <stdexcept>

namespace xmlPrs {
/** @brief A runtime error that can be raised when using any object in xmlPrs::
 */
class Error : public std::runtime_error {
public:
  explicit Error(const std::string &what);
};
} // namespace xmlPrs
