#ifdef ENABLE_XML_Parser_nlohmann_json_converter
#include <XML-Parser/Converter.h>
#include <variant>
#include <algorithm>

namespace xmlPrs {
    namespace {
        template<typename T>
        std::unordered_multimap<Name, std::vector<const T*>> compute_clusters(const std::unordered_multimap<Name, T>& subject) {
            std::unordered_multimap<Name, std::vector<const T*>> result;
            auto it = subject.begin();
            while (it != subject.end()) {
                auto it_next = it;
                while ((it_next != subject.end()) && (it_next->first == it->first)) {
                    ++it_next;
                }
                std::vector<const T*> values;
                std::for_each(it, it_next, [&values](const std::pair<const Name, T>& element) {
                    values.push_back(&element.second);
                });
                result.emplace(it->first, std::move(values));
                it = it_next;
            }
            return result;
        };

        void to_json(nlohmann::json& recipient, const Attributes& attributes) {
            auto clusters = compute_clusters(attributes);
            for (const auto& [name, values] : clusters) {
                if (1 == values.size()) {
                    recipient[name] = *values.front();
                }
                else {
                    std::vector<std::string> values_copies;
                    values_copies.reserve(values.size());
                    for (const auto* value : values) {
                        values_copies.push_back(*value);
                    }
                    recipient[name] = values_copies;
                }
            }
        }

        void to_json(nlohmann::json& recipient, const Tags& nested) {
            auto clusters = compute_clusters(nested);
            for (const auto& [name, values] : clusters) {
                if (1 == values.size()) {
                    auto& new_tag = recipient[name];
                    to_json(new_tag, (*values.front())->getAttributes());
                    to_json(new_tag, (*values.front())->getNested());
                }
                else {
                    auto nested_array = nlohmann::json::array();
                    for (const auto* value : values) {
                        auto& new_tag = nested_array.emplace_back();
                        to_json(new_tag, (*value)->getAttributes());
                        to_json(new_tag, (*value)->getNested());
                    }
                }
            }
        }

        void to_json(nlohmann::json& recipient, const Tag& tag) {
            if (!tag.getAttributes().empty()) {
                to_json(recipient["attributes"], tag.getAttributes());
            }
            if (!tag.getNested().empty()) {
                to_json(recipient["nested"], tag.getNested());
            }
        }
    }

    void to_json(nlohmann::json& recipient, const Root& root) {
        to_json(recipient[root.getName()], static_cast<const Tag&>(root));
    }
}

#endif
