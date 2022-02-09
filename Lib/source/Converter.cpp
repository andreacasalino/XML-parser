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

        void append_attributes(nlohmann::json& recipient, const Attributes& attributes) {
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

        void append_nested(nlohmann::json& recipient, const Tags& nested) {
            auto clusters = compute_clusters(nested);
            for (const auto& [name, values] : clusters) {
                if (1 == values.size()) {
                    auto& new_tag = recipient[name];
                    append_attributes(new_tag, (*values.front())->getAttributes());
                    append_nested(new_tag, (*values.front())->getNested());
                }
                else {
                    auto nested_array = nlohmann::json::array();
                    for (const auto* value : values) {
                        auto& new_tag = nested_array.emplace_back();
                        append_attributes(new_tag, (*value)->getAttributes());
                        append_nested(new_tag, (*value)->getNested());
                    }
                }
            }
        }

        void append_tag_content(nlohmann::json& recipient, const Tag& tag) {
            if (!tag.getAttributes().empty()) {
                append_attributes(recipient["attributes"], tag.getAttributes());
            }
            if (!tag.getNested().empty()) {
                append_nested(recipient["nested"], tag.getNested());
            }
        }
    }

    nlohmann::json to_json(const Root& xml_structure) {
        nlohmann::json result;
        append_tag_content(result[xml_structure.getName()], xml_structure);
        return result;
    }
}

#endif
