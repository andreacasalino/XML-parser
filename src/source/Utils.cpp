/**
 * Author:    Andrea Casalino
 * Created:   04.09.2024
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include <Utils.h>

namespace xmlPrs {
std::string ViewsCollection::toString() const {
  std::string res;
  for (const auto &value : values) {
    res += value;
  }
  std::string_view res_view{res.data()};
  stripL<' ', '\n', '\t'>(res_view);
  if (res_view.empty()) {
    res.clear();
  }
  return res;
}
} // namespace xmlPrs
