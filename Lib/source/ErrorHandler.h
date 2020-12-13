/**
 * Author:    Andrea Casalino
 * Created:   03.12.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#pragma once

#ifndef _XML_ERROR_HANDLER_H_
#define _XML_ERROR_HANDLER_H_

#include <string>
#include <mutex>

namespace xmlPrs {
    class ErrorHandler {
    public:
        static void handle(const std::string& what);
        static void useThrow();
        static void useCout();
    private:
        static std::mutex mtxError;
        static bool useThrowFlag;
    };
}

#endif