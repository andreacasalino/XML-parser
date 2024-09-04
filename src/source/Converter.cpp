/**
 * Author:    Andrea Casalino
 * Created:   04.09.2024
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifdef ENABLE_XML_Parser_nlohmann_json_converter
#include <XML-Parser/Converter.h>
#include <algorithm>
#include <variant>

namespace xmlPrs {
namespace {
template<typename Ptr, typename Pred, typename Container>
std::unordered_map<std::string_view, std::vector<const Ptr*>> clusters(const Container& container, Pred pred) {
  std::unordered_map<std::string_view, std::vector<const Ptr*>> res;
  for(const auto& [k, v] : container) {
    res[k.data()].push_back(pred(v));
  }
  return res;
}

void to_json(nlohmann::json &recipient, const Attributes &attributes) {
  std::unordered_map<std::string_view, std::vector<const std::string*>> map = clusters<std::string>(attributes, [](const std::string& str){
    return &str;
  });
  for (const auto &[name, values] : map) {
    if (1 == values.size()) {
      recipient[std::string{name}] = *values.front();
    } else {
      std::vector<std::string> values_copies;
      values_copies.reserve(values.size());
      for (const auto *value : values) {
        values_copies.push_back(*value);
      }
      recipient[std::string{name}] = values_copies;
    }
  }
}

void to_json(nlohmann::json &recipient, const Tags &nested) {
  std::unordered_map<std::string_view, std::vector<const Tag*>> map = clusters<Tag>(nested, [](const TagPtr& nested){
    return nested.get();
  });
  for (const auto &[name, values] : map) {
    if (1 == values.size()) {
      to_json(recipient[std::string{name}], *values.front());
    } else {
      auto& slot = recipient[std::string{name}];
      slot = nlohmann::json::array();
      for (const auto *value : values) {
        to_json(slot.emplace_back(), *value);
      }
    }
  }
}
} // namespace

void to_json(nlohmann::json &recipient, const Tag &tag) {
  recipient["name"] = tag.getName();
  const auto& content = tag.getContent();
  if(!content.empty()) {
    recipient["content"] = content;
  }
  to_json(recipient["attributes"], tag.getAttributes());
  to_json(recipient["nested"], tag.getNested());
}
} // namespace xmlPrs

#endif
