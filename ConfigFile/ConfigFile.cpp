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
//  ConfigFile.cpp
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

#include "ConfigFile.hpp"

// for file reading
#include <fstream>
// for file reading and std output
#include <iostream>
// for ERROR
#include <ErrorManager.hpp>
// for string conversion methods
#include <string>
// for rename
#include <cstdio>
// for error management
#include <stdexcept>

#include <cstdlib>

#include <sstream>

template <typename T>
std::string ToString(T val)
{
    std::stringstream stream;
    stream << val;
    return stream.str();
}


// constructor for the config file
// @param filepath to the configuration file.
ConfigFile::ConfigFile(string path)
{
    filepath = path;
}


#define START_STATE 0
#define VARIABLE_STATE 1
#define EQUAL_STATE 2
#define VALUE_NAME_STATE 3
#define END_STATE 4

// parseLine - parses a single line of the file
//
// @param line - the line to be parsed
// @param varName - an empty string to fill the variable name with.
// @param value - an empty string to fill the value with.
// @return - 0 if it completes with a valid statement, 1 if it parses correctly without a statement,
//           -1 if it fails to parse
int ConfigFile::parseLine(string line, string &varName, string &value, string &comment)
{
    short state = START_STATE;
    //string varName;
    //string value;

    //cout << "Line = " << line << endl;

    for (unsigned int i = 0; i < line.length(); i++)
    {
        char c = line[i];
        // check if comment
        if (c == '#') {
            // attach the rest of the line from then on into the comment section
            comment = line.substr(i, 80000); // 80000 for more then you would put on a single line
            break;
        }

        //cout << "i = " << i << ", c = " << c << " state = " << state << endl;
        switch (state) {
            case START_STATE :
                if (!isspace(c)) {
                    state = VARIABLE_STATE;
                    varName.push_back(c);
                } // end is space
                break;
            case VARIABLE_STATE :
                if (isspace(c) || c == '=') {
                    state = EQUAL_STATE;
                } else {
                    varName.push_back(c);
                } // end if else
                break;
            case EQUAL_STATE :
                if (!(isspace(c) || c == '=')) {
                    value.push_back(c);
                    state = VALUE_NAME_STATE;
                }
                break;
            case VALUE_NAME_STATE :
                if (isspace(c) || c == '=') {
                    if (c == '=') return -1;
                    state = END_STATE;
                } else {
                    value.push_back(c);
                }

                break;
            case END_STATE :
                if (!isspace(c)) return -1;
                break;
            default:
                // TODO handle error
                break;
        } // end switch
    } // end for loop

    if (state == VALUE_NAME_STATE || state == END_STATE) {
        //vars[varName] = value; Removed line and store instead in parameter.
        return 0; // finished line with complete statement
    } else {
        if (state != START_STATE)
            return -1; // line finished with a parse error
        return 1; // line finished with no parse error
    }
}


// reloads the configuration file
// @return - 0 on success, -1 on failure.
int ConfigFile::load()
{
    ifstream iFile;

    iFile.open(filepath, ios::in);
    if (!iFile.is_open()) {
        // TODO: Deal with error case of no input!
        cout << "Couldn't read file" << endl;
        return -1;
    }
    string line;

    // go through each line and parse it.
    while (getline(iFile, line))
    {
        string varName;
        string value;
        string comment;

        int ret = parseLine(line, varName, value, comment);
        if (ret < 0) {
            // TODO handle failure
            //cerr << "Line parse failed... recording error" << endl;
            ErrorManager::ERROR(ERROR_READING_CONFIG_FILE);
            return -1;
        } else if (ret == 0) {
            vars[varName] = value;
        }
    } // end while loop for parsing

    iFile.close();
    return 0;
}


#define TEMP_CONFIG_FILE "tmp.inca"

int ConfigFile::save()
{
    ifstream iFile;
    ofstream oFile;

    std::string tempConfigFile = filepath + std::string(TEMP_CONFIG_FILE);
    oFile.open(tempConfigFile.c_str());
    if (!oFile.is_open()) {
        // TODO: Deal with error case of not being able to open file
        cout << "Couldn't open file to save to." << endl;
        return -1;
    }

    string line;
    unordered_map<string, bool> savedValues;

    iFile.open(filepath);
    if (iFile.is_open()) {
        // If it can't open the original file, then
        // only output the data and don't worry about the old file
        // as it is possible a new file would want to be created.

        // go through each line and parse it to see if it needs to be saved to the new save file
        while (getline(iFile, line)) {
            string varName;
            string value;
            string comment;

            int ret = parseLine(line, varName, value, comment);

            if (ret == 0) {
                // valid value found
                // ignore any values found that are not currently in the RAM config file
                if (vars.count(varName) > 0) {
                    oFile << varName << " = " << vars[varName] << " " << comment << endl;
                    savedValues[varName] = true;
                }
            } else if (ret == 1) {
                // line parsed correctly, but no value read.
                oFile << comment << endl;
            } else {
                // line parsed incorrectly.
                // handle by ignoring current line. Post error.
                ErrorManager::ERROR(CONFIG_FILE_SAVE_FOUND_BAD_LINE_IGNORING);
                //cerr << "ERROR: Config file save found bad line ignoring..." << endl;
            } // end if else statements
        } // end while loop
    } // end if for checking if orginal file exists at all.



    // read through all values in config file, and store any new values not found
    for (unordered_map<string, string>::iterator itr = vars.begin(); itr != vars.end(); ++itr) {
        if (savedValues.count(itr->first) == 0) {
            //cout << "Hasn't encountered " << itr->first << endl;
            //cout << itr->first << " = " << itr->second << "\n";
            oFile << endl << itr->first << " = " << itr->second << endl;
        }
    }

    iFile.close();
    oFile.close();

    if (rename(tempConfigFile.c_str(), filepath.c_str()) != 0) {
        cerr << "File rename failed..." << endl;
        ErrorManager::ERROR(CONFIG_FILE_FAILED_TO_RENAME_FILE);
        return -1;
    }

    return 0;
} // end save function


// setString - function sets the given varName to the string given.
// If the variable already exists, it modifies the current value.
// If the variable does not exist, then it creates a new variable.
// You must call the save() function after a set call before it gets saved to disk
// @param varName - the variable name in the config file.
// @param var - the variable to be stored
//
// @return - 0 for no error
int ConfigFile::setString(string varName, string var) {
    vars[varName] = var;

    return 0;
}
// setDouble function sets the given varName to the variable given, but it must
// first convert the given value to a string before storing it.
// If the variable already exists, it modifies the current value.
// If the variable does not exist, then it creates a new variable.
// You must call the save() function after a set call before it gets saved to disk
// @param varName - the variable name in the config fil
// @param var - the variable to be stored
//
// @return - 0 for no error
int ConfigFile::setDouble(string varName, double var) {
    vars[varName] = ToString(var);

    return 0;
}
int ConfigFile::setInt(string varName, int var) {
    vars[varName] = ToString(var);

    return 0;
}
int ConfigFile::setLong(string varName, long var) {
   vars[varName] = ToString(var);

   return 0;
}

// getString - function finds the given varName and returns the string version of the value.
// @param varName - the variable name in the config file.
// @param var - the pointer to the variable returned by the function.
//
// @return - 0 for no error, 1 for unable to find variable in config file.
int ConfigFile::getString(string varName, string *var) {
    if (vars.count(varName) != 1) {
        ErrorManager::ERROR(UNABLE_TO_FIND_VARIABLE_IN_CONFIG_FILE);
        return 1;
    }

    *var = vars[varName];

    return 0;
}



// get conversion functions these will return the variable as the second argument
// All of the values are stored as strings, these are built in conversion functions
// all of them are based around the same set of code.
// All function use the getString function to get the string version of the requested value.
// @param varName - the variable name in the config file.
// @param var - the pointer to the variable returned by the function.
//
// @return - 0 for no error, 1 for unable to find variable in config file.
//              2 - bad value and could not convert to type, 3, value out of range
int ConfigFile::getDouble(string varName, double *var) {
    string str;
    if (getString(varName, &str) != 0) {
        // handle error
        return 1;
    }

    *var = strtod(str.c_str(), NULL);
    // TODO: handle errors???
    /*
    try {
        *var = stod(str, NULL);
    } catch (const invalid_argument &exception) {
        ErrorManager::ERROR(CONFIG_FILE_READ_DOUBLE_INVALID_VALUE);
        return 2;
    } catch (const out_of_range &exception) {
        ErrorManager::ERROR(CONFIG_FILE_READ_DOUBLE_OUT_OF_RANGE);
        return 3;
    }
    */

    return 0;
}
int ConfigFile::getFloat(string varName, float *var) {
    string str;
    if (getString(varName, &str) != 0) {
        // handle error
        return 1;
    }

    *var = strtof(str.c_str(), NULL);
    // TODO: handle errors???
    /*
    try {
        *var = stof(str, NULL);
    } catch (const invalid_argument &exception) {
        ErrorManager::ERROR(CONFIG_FILE_READ_FLOAT_INVALID_VALUE);
        return 2;
    } catch (const out_of_range &exception) {
        ErrorManager::ERROR(CONFIG_FILE_READ_FLOAT_OUT_OF_RANGE);
        return 3;
    }*/

    return 0;
}
int ConfigFile::getInt(string varName, int *var) {
    string str;
    if (getString(varName, &str) != 0) {
        // handle error
        return 1;
    }

    *var = (int)strtod(str.c_str(), NULL);
    // TODO: handle errors???
    /*
    try {
        // only use Decimal numbers in config file
        *var = stoi(str, NULL);
    } catch (const invalid_argument &exception) {
        ErrorManager::ERROR(CONFIG_FILE_READ_INT_INVALID_VALUE);
        return 2;
    } catch (const out_of_range &exception) {
        ErrorManager::ERROR(CONFIG_FILE_READ_INT_OUT_OF_RANGE);
        return 3;
    }*/

    return 0;
}
int ConfigFile::getHex(string varName, int *var) {
    string str;
    if (getString(varName, &str) != 0) {
        // handle error
        return 1;
    }

    *var = (int)strtod(str.c_str(), NULL);
    // TODO: handle errors???

    /*
    try {
        // only use Decimal numbers in config file
        *var = stoi(str, NULL, 16);
    } catch (const invalid_argument &exception) {
        ErrorManager::ERROR(CONFIG_FILE_READ_INT_INVALID_VALUE);
        return 2;
    } catch (const out_of_range &exception) {
        ErrorManager::ERROR(CONFIG_FILE_READ_INT_OUT_OF_RANGE);
        return 3;
    }*/

    return 0;
}
int ConfigFile::getLong(string varName, long *var) {
   string str;
   if (getString(varName, &str) != 0) {
       // handle error
       return 1;
   }

   *var = (int)strtod(str.c_str(), NULL);
    // TODO: handle errors???

   /*
   try {
       // only use Decimal numbers in config file
       *var = stol(str, NULL);
   } catch (const invalid_argument &exception) {
       ErrorManager::ERROR(CONFIG_FILE_READ_INT_INVALID_VALUE);
       return 2;
   } catch (const out_of_range &exception) {
       ErrorManager::ERROR(CONFIG_FILE_READ_INT_OUT_OF_RANGE);
       return 3;
   }*/

   return 0;
}

// this function will go through the map and output all of the key-value pairs
// this probably shouldn't be called in flight.
void ConfigFile::print() {
    for (unordered_map<string, string>::iterator itr = vars.begin(); itr != vars.end(); ++itr) {
        cout << "key: " << itr->first << " value: " << itr->second << endl;
    }
}




// checkElementsAndKeys
// This is a test function to check if all of the keys and values are in the map
// if there are any other elements this function will also fail
// NOTE: since this is a test function if will be Verbose.
// NOTE: NOT DESIGNED TO BE RAN ON THE SATELLITE
//
// @param keys - the set of keys to check are in the map
// @param values - the set of values to check are in the map.
//
// @return true if it passes, false if it fails.
bool ConfigFile::checkElementsAndKeys(string *keys, string *values, int length)
{
    if ((unsigned int)length != vars.size()) {
        cout << "ERROR: ConfigFile Test: map size = " << vars.size() << " test length = " << length << " do not match..." << endl;
        return false;
    }

    for (int i = 0; i < length; i++) {
        if (vars.count(keys[i]) != 1) {
            return false;
        }
        //cout << "vars[keys[i]] = " << vars[keys[i]] << " values[i] = " << values[i] << std::endl;
        if (vars[keys[i]] != values[i]) {
            cout << "ERROR: ConfigFile Test: The values in map do not match..." << endl;
            return false;
        }
    }

    return true;
} // end checkElementAndKeys
