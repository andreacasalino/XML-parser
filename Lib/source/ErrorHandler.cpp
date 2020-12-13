/**
 * Author:    Andrea Casalino
 * Created:   03.12.2019
 *
 * report any bug to andrecasa91@gmail.com.
 **/

#include "ErrorHandler.h"
#include <ParserError.h>
#include <iostream>

namespace xmlPrs {
    std::mutex ErrorHandler::mtxError;
    bool ErrorHandler::useThrowFlag =  true;

    void ErrorHandler::handle(const std::string& what){
        std::lock_guard<std::mutex> lk(mtxError);
        if(useThrowFlag) throw ParserError(what);
        std::cout << what << std::endl; 
    }

    void ErrorHandler::useThrow() {
        std::lock_guard<std::mutex> lk(mtxError);
        useThrowFlag = true;
    };
    
    void ErrorHandler::useCout() {
        std::lock_guard<std::mutex> lk(mtxError);
        useThrowFlag = false;
    };
}
