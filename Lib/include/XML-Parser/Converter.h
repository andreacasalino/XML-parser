/**
 * Author:    Andrea Casalino
 * Created:   03.12.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifdef ENABLE_XML_Parser_nlohmann_json_converter
#pragma once

#include <XML-Parser/Tag.h>
#include <nlohmann/json.hpp>

namespace xmlPrs {
    void to_json(nlohmann::json& recipient, const Root& root);
}

#endif
