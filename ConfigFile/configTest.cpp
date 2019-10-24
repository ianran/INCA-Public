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
//
//  ConfigTest.cpp
//  Created by Ian Rankin on 8/14/17.
//
// This is the set of test code for the ConfigFile class.

#include <iostream>
#include "ConfigFile.hpp"
#include <cstdio>

using namespace std;

string test1Keys[3] = {"RandomVariable", "startBatteryVoltage", "messageToShow"};
string test1Values[3] = {"45", "3.7", "HelloYouSucker"};

string test2Keys[4] = {"a", "b", "c", "GOOD_ADAAG"};
string test2Values[4] = {"3456.32552", "2", "-24.567", "bob"};

string test4_1Keys[2] = {"abc", "b"};
string test4_1Values[2] = {"forty", "2"};

string test5Keys[6] = {"var1", "var2", "hey", "neg", "negD", "n"};
string test5Values[6] = {"helloWorld", "byeWorld", "45.95", "-2", "-2.045", "3"};




int main(void) {
    int ret;
    int retGets;


    // open and load the example config file
    /////////////////////////////////////////// Test 1
    ConfigFile ex("ExampleConfigFile.inca");
    ret = ex.load();
    retGets = 0;

    int numFailed = 0;

    int ranInt;
    float ranFloat;
    double ranDouble;

    float batVoltF;
    double batVoltD;

    // no error = 0 so if addition is not 0 there was an error
    retGets = ex.getInt("RandomVariable", &ranInt);
    retGets += ex.getFloat("RandomVariable", &ranFloat);
    retGets += ex.getDouble("RandomVariable", &ranDouble);

    retGets += ex.getFloat("startBatteryVoltage", &batVoltF);
    retGets += ex.getDouble("startBatteryVoltage", &batVoltD);

    cout << "TEST  - [ExampleConfigFile.inca]" << endl;
    if (ex.checkElementsAndKeys(test1Keys, test1Values, 3) && ret == 0)
    {
        cout << "Passed - read file" << endl;
    } else {
        cout << "Failed - read file test" << endl;
        numFailed++;
    }
    if (retGets == 0) { cout << "Passed - retGets" << endl; }
    else { cout << "Failed - retGets" << endl; numFailed++; }
    if (ranInt == 45 && ranFloat == 45.0 && ranDouble == 45.0) { cout << "Passed - RandomVariable checks" << endl; }
    else { cout << "Failed - RandomVariable checks" << endl; numFailed++; }
    if (batVoltF == 3.7f && batVoltD == 3.7) { cout << "Passed - batVolt checks" << endl; }
    else { cout << "Failed - batVolt checks" << endl; numFailed++; }
    //cout << "ret = " << ret << " retGets = " << retGets << " ranInt = " << ranInt << " ranFloat = " << ranFloat << " batVoltF = " << batVoltF << " batVoltD = " << batVoltD << endl;


    /////////////////////////////////////////// Test 2
    ConfigFile test2("testConfigFiles/TestConfig2.inca");
    ret = test2.load();
    retGets = 0;

    double a;
    int b;
    float c;
    string d;

    // no error = 0 so if addition is not 0 there was an error
    retGets += test2.getDouble("a", &a);

    retGets += test2.getInt("b", &b);
    retGets += test2.getFloat("c", &c);
    //retGets += ex.getString("GOOD_ADAAG", &d);

    // test print
    //ex.print();

    cout << "TEST  - [TestConfig2]" << endl;
    if (test2.checkElementsAndKeys(test2Keys, test2Values, 4) && ret == 0)
    {
        cout << "Passed - read file" << endl;
    } else {
        cout << "Failed - read file test" << endl;
        numFailed++;
    }
    if (retGets == 0) { cout << "Passed - retGets" << endl; }
    else { cout << "Failed - retGets" << endl; numFailed++; }
    if (a == 3456.32552 && b == 2 && c == -24.567f) { cout << "Passed - read number checks" << endl; }
    else { cout << "Failed - read number checks" << endl; numFailed++; }


    /////////////////////////////////////////// Test 3 (Should not parse correctly...)
    ConfigFile test3("testConfigFiles/TestConfig3.inca");
    ret = test3.load();

    if (ret != 0) {
        cout << "Passed - [TestConfig3.inca] test" << endl;
    } else {
        cout << "Failed - [TestConfig3.inca] test" << endl;
        numFailed++;
    }

    /////////////////////////////////////////// Test 4 (Should not parse correctly...)
    ConfigFile test4("testConfigFiles/TestConfig4.inca");
    ret = test4.load();
    ConfigFile test4sec("testConfigFiles/TestConfig4-1.inca");
    int ret2 = test4sec.load();

    bool ret4 = test4sec.checkElementsAndKeys(test4_1Keys, test4_1Values, 2);
    if (!ret4) { cout << "Failed first check of elements and keys..." << endl; }
    // save file then reload it again to check it hasn't changed.
    ret2 += test4sec.save();
    ret2 += test4sec.load();
    bool ret3 = test4sec.checkElementsAndKeys(test4_1Keys, test4_1Values, 2);
    if (!ret3) { cout << "Failed second check of elements and keys..." << endl; }

    if (ret != 0 && ret2 == 0 && ret3 && ret4) {
        cout << "Passed - [TestConfig4.inca] test" << endl;
    } else {
        cout << "Failed - [TestConfig4.inca] test" << endl;
        numFailed++;
    }


    //////////////////////////////////////////// Test 5 should generate readable file
    ConfigFile test5("testConfigFiles/newConfigFile.inca");
    test5.setString("var1", "helloWorld");
    test5.setString("var2", "byeWorld");
    test5.setDouble("hey", 45.95);
    test5.setInt("n", 3);
    test5.setInt("neg", -2);
    test5.setDouble("negD", -2.045);
    //test5.print();

    test5.save();
    ConfigFile test5_1("testConfigFiles/newConfigFile.inca");
    ret = test5_1.load();

    ret3 = test5_1.checkElementsAndKeys(test5Keys, test5Values, 6);
    if (ret == 0 && ret3) {
        cout << "Passed - [TestConfig5.inca] test" << endl;
    } else {
        cout << "Failed - [TestConfig5.inca] test" << endl;
        numFailed++;
    }

    test5.setInt("n", 5);
    test5.save();

    test5Values[5] = "5";

    ConfigFile test5_2("testConfigFiles/newConfigFile.inca");
    ret = test5_2.load();

    ret3 = test5_2.checkElementsAndKeys(test5Keys, test5Values, 6);
    if (ret == 0 && ret3) {
        cout << "Passed - [TestConfig5.inca] test, update single value" << endl;
    } else {
        cout << "Failed - [TestConfig5.inca] test, update single value" << endl;
        numFailed++;
    }

    // comment out line below to check if output is correct
    remove("testConfigFiles/newConfigFile.inca");

    ////////////////////////////////////////// Print tests results
    if (numFailed == 0) {
        cout << "ALL ConfigFile TESTS PASSED!" << endl;
        return 0;
    }
    else {
        cout << "FAILED - Failed " << numFailed << " ConfigFile Test Failed..." << endl;
        return -numFailed;
    }
}
