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
#include <sstream>

namespace xmlPrs {
    /** @brief A runtime error that can be raised when using any object in xmlPrs::
	 */
    class ParserError : public std::runtime_error {
    public:
        explicit ParserError(const std::string& what);
    };

    /** @brief After calling this function, any problems encountered when using the 
     * objects in xmlPrs:: trigger the throw of a ParserError.
	 */
    void UseThrowError();

    /** @brief After calling this function, any problems encountered when using the 
     * objects in xmlPrs:: are printed using cout, without raising a ParserError exception
	 */
    void UseInformativeCout();
}

#endif
