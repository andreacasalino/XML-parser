/**
 * Author:    Andrea Casalino
 * Created:   04.09.2024
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifdef ENABLE_XML_Parser_nlohmann_json_converter
#pragma once

#include <XML-Parser/Tag.h>
#include <nlohmann/json.hpp>

namespace xmlPrs {
    void to_json(nlohmann::json& recipient, const Tag& tag);
}

#endif
