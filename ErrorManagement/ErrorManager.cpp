//// Copyright © 2019 New Mexico State Univeristy
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the “Software”), to deal in
// the Software without restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
// Software, and to permit persons to whom the Software is furnished to
// do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//  ErrorManager.cpp
//
//  Created by Ian Rankin on 12/12/16.
//

#include "ErrorManager.hpp"

// manager instance for error handler, for singleton class.
static ErrorManager *managerInstance;

// get Error manager singlton for error management
// Function does lazy instantion of the errorManager
// for getting the singleton class
ErrorManager *ErrorManager::getErrorManager() {
    if (managerInstance == NULL) {
        managerInstance = new ErrorManager();
    }

    return managerInstance;
}


// a simple function for telling to log an error.
void ErrorManager::ERROR(int errorType) {
    ErrorManager::getErrorManager()->error(errorType);
}

// Tells the ErrorManager to log an error.
// the type is specified as an integer which is enumerated in
// list of errors in ErrorManager.hpp
void ErrorManager::error(int errorType) {
    std::cout << "Error of type: " << errorType << std::endl;
}




// private constructor
// The constructor is private to use singleton design pattern.
ErrorManager::ErrorManager() {

}




// getMostCriricalError returns the error with the highest critical error.
Error ErrorManager::getMostCriticalError()
{
    return *mostCriticalError; // make sure it exits...
}
