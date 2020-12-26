/**
 * Author:    Andrea Casalino
 * Created:   03.12.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#ifndef _XML_ERROR_H_
#define _XML_ERROR_H_

#include <stdexcept>
#include <sstream>

namespace xmlPrs {
    /** @brief A runtime error that can be raised when using any object in xmlPrs::
	 */
    class Error : public std::runtime_error {
    public:
        explicit Error(const std::string& what);

        template<typename ... Args>
        Error(Args ... args)
            : Error(MergeArgs(args...)) {
        };
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
        static void mergeArgs(std::stringstream& stream, const std::string& a);
        template<typename ... Args>
        static void mergeArgs(std::stringstream& stream, const char* a, Args ... args){
            stream << a;
            mergeArgs(stream, args...);
        };
        static void mergeArgs(std::stringstream& stream, const char* a);
    };
}

#endif
