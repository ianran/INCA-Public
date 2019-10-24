// Copyright © 2019 New Mexico State Univeristy
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
//
//  ConfigFile.hpp
//  Created by Ian Rankin on 8/14/17.
//
// This set of code is designed to handle any constants needed for the satellite
// this file loads a changable configuration file and then allows fast access to any
// variable inside of the config file
// The variable can be int's, numbers, strings, with only a little extra code required
// if another data type is requried.
// The class can handle both loading and saving to the config file.
//
// Example code for use is shown below:
//
// ConfigFile config("pathToConfigFile");
// // load required before trying to access any variables.
// config.load()
// double x;
//
// if (config.getDouble("x", &x) == 0) {
// // handle error of no x in map
// }
//
// // x should now have value that was stored in config file

#ifndef ConfigFile_hpp
#define ConfigFile_hpp

#include <unordered_map>
#include <vector>
#include <string>


using namespace std;

template <typename T>
std::string ToString(T val);

class ConfigFile {
public:

    // constructor for the config file
    // @param filepath to the configuration file.
    ConfigFile(string path);

    // reloads the configuration file
    // @return - 0 on success, -1 on failure.
    int load();
    int save();

    // getString - function finds the given varName and returns the string version of the value.
    // @param varName - the variable name in the config file.
    // @param var - the pointer to the variable returned by the function.
    //
    // @return - 0 for no error, 1 for unable to find variable in config file.
    int getString(string varName, string *var);

    // get conversion functions these will return the variable as the second argument
    // All of the values are stored as strings, these are built in conversion functions
    // all of them are based around the same set of code.
    // All function use the getString function to get the string version of the requested value.
    // @param varName - the variable name in the config file.
    // @param var - the pointer to the variable returned by the function.
    //
    // @return - 0 for no error, 1 for unable to find variable in config file.
    //              2 - bad value and could not convert to type, 3, value out of range
    int getDouble(string varName, double *var);
    int getFloat(string varName, float *var);
    int getInt(string varName, int *var);
    int getHex(string varName, int *var);
    int getLong(string varName, long *var);

    // set function, these will change the variable given
    int setDouble(string varName, double var);
    int setString(string varName, string var);
    int setInt(string varName, int var);
    int setLong(string varName, long var);

    // this function will go through the map and output all of the key-value pairs
    // this probably shouldn't be called in flight.
    void print();

    // checkElementsAndKeys
    // This is a test function to check if all of the keys and values are in the map
    // if there are any other elements this function will also fail
    // NOTE: since this is a test function it will be Verbose.
    // NOTE: NOT DESIGNED TO BE RAN ON THE SATELLITE
    //
    // @param keys - the set of keys to check are in the map
    // @param values - the set of values to check are in the map.
    //
    // @return true if it passes, false if it fails.
    bool checkElementsAndKeys(string *keys, string *values, int length);

private:
    // map of variables.
    unordered_map<string, string> vars;

    string filepath;



    // parseLine - parses a single line of the file
    //
    // @param line - the line to be parsed
    // @param varName - an empty string to fill the variable name with.
    // @param value - an empty string to fill the value with.
    // @return - 0 if it completes with a valid statement, 1 if it parses correctly without a statement,
    //           -1 if it fails to parse
    int parseLine(string line, string &varName, string &value, string &comment);
};

#endif /* ConfigFile_hpp */
