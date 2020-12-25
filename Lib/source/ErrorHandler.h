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
#include <sstream>

namespace xmlPrs {
    class ErrorHandler {
    public:
        static void handle(const std::string& what);
        
        template<typename ... Args>
        static void handle(Args ... args){
            handle(MergeArgs(args...));
        };

        static void useThrow();
        static void useCout();
    private:
        template<typename ... Args>
        static std::string MergeArgs(Args ... args){
            std::stringstream stream;
            mergeArgs(stream, args...);
            return stream.str();
        };
        template<typename ... Args>
        static void mergeArgs(std::stringstream& stream, const std::string& a, Args ... args){
            stream << a;
            mergeArgs(stream, args...);
        };
        static inline void mergeArgs(std::stringstream& stream, const std::string& a){ stream << a; };
        template<typename ... Args>
        static void mergeArgs(std::stringstream& stream, const char* a, Args ... args){
            stream << a;
            mergeArgs(stream, args...);
        };
        static inline void mergeArgs(std::stringstream& stream, const char* a){ stream << a; };

        static std::mutex mtxError;
        static bool useThrowFlag;
    };
}

#endif