/**
 * Author:    Andrea Casalino
 * Created:   03.12.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#ifndef _XML_PARSER_ERROR_H_
#define _XML_PARSER_ERROR_H_

#include <stdexcept>

namespace xmlPrs {
    class ParserError : public std::runtime_error {
    public:
        explicit ParserError(const std::string& what);
    };

    void UseThrowError();

    void UseInformativeCout();
}

#endif
