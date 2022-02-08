#ifdef ENABLE_XML_Parser_nlohmann_json_converter
#include <XML-Parser/Converter.h>
#include <variant>
#include <algorithm>

namespace xmlPrs {
    namespace {
        template<typename T>
        struct ClusterOfEquallyNamedValue {
            const Name* name;
            std::vector<const T*> values;
        };

        template<typename T>
        std::unordered_multimap<Name, T>::const_iterator find_first_different(
            const std::unordered_multimap<Name, T>::const_iterator& start,
            const std::unordered_multimap<Name, T>::const_iterator& end) {
            for (auto it = start; it != end; ++it) {
                if (it->first != start->first) {
                    return it;
                }
            }
            return end;
        }

        template<typename T>
        std::vector<ClusterOfEquallyNamedValue<T>> compute_clusters(const std::unordered_multimap<Name, T>& map) {
            if (map.empty()) {
                return {};
            }
            std::vector<ClusterOfEquallyNamedValue<T>> result;
            result.reserve(map.size());
            auto it = map.begin();
            while (it != map.end()) {
                auto it_next = find_first_different(it, map.end());
                result.emplace_back(ClusterOfEquallyNamedValue{ &it->first, std::vector<const T*>{} });
                std::for_each(it, it_next, [&cluster = result.back()](const std::pair<Name, T>& element) {
                    cluster.values.push_back(&element.second);
                });
                it = it_next;
            }
            return result;
        }

        Attributes::const_iterator find_first_different_name(const Attributes::const_iterator& actual, const Attributes::const_iterator& end) {
            for (auto it = actual; it != end; ++it) {
                if (it->first != actual->first) {
                    return it;
                }
            }
            return end;
        }

        void append_attributes(nlohmann::json& recipient, const Attributes& attributes) {
            auto clusters = compute_clusters(attributes);
            for (const auto& cluster : clusters) {
                if (1 == cluster.values.size()) {
                    recipient[*cluster.name] = *cluster.values.front();
                }
                else {
                    std::vector<std::string> values_copies;
                    values_copies.reserve(cluster.values.size());
                    for (const auto* value : cluster.values) {
                        values_copies.push_back(*value);
                    }
                    recipient[*cluster.name] = values_copies;
                }
            }
        }

        void append_nested(nlohmann::json& recipient, const Tags& nested) {
            auto nested_clusters = compute_clusters(nested);
            for (const auto& cluster : nested_clusters) {
                if (1 == cluster.values.size()) {
                    auto& new_tag = recipient[*cluster.name];
                    append_attributes(new_tag, (*cluster.values.front())->getAttributes());
                    append_nested(new_tag, (*cluster.values.front())->getNested());
                }
                else
                    auto nested_array = nlohmann::json::array();
                    for (const auto* value : cluster.values) {
                        auto& new_tag = nested_array.push_back
                        append_attributes(new_tag, (*value)->getAttributes());
                        append_nested(new_tag, (*value)->getNested());
                    }
                }
            }
        }

        void append_tag_content(nlohmann::json& recipient, const Tag& tag) {
            append_attributes(recipient["attributes"], tag.getAttributes());
            append_nested(recipient["nested"], tag.getNested());
        }
    }

    nlohmann::json to_json(const Root& xml_structure) {
        nlohmann::json result;
        append_tag_content(result[xml_structure.getName()], xml_structure);
        return result;
    }
}

#endif
