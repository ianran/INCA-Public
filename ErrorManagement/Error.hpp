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
//  Error.hpp
//
//  Created by Ian Rankin on 12/14/16.
//

#ifndef Error_hpp
#define Error_hpp

#include <iostream>
#include <cstdlib>

class Error {
public:
    // FIX Constructor
    Error(int ErrorType, time_t TimeError);

    // returns the error type stored in the error.
    // For a list of errors look at ErrorManager.hpp
    int getErrorType();
    time_t getTimeOfError();




private:
    int errorType;
    time_t timeError;
};

#endif /* Error_hpp */
