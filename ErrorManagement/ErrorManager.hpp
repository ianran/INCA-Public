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
//  ErrorManager.hpp
//
//  Created by Ian Rankin on 12/12/16.
//
// This is a class to handle errors from any part of the code.
// It will signal the state_machine to go to safe mode if needed,
// and will log all errors.
//
// The class itself is a singleton type to allow only one instation of the
// object. A generic use of this class is:
//
// ErrorManager::ERROR(STATE_MACHINE_PTHREAD_FAILED_TO_INIT)
//
//

#ifndef ErrorManager_hpp
#define ErrorManager_hpp

#include <iostream>
#include <vector>
#include "Error.hpp"

using namespace std;



class ErrorManager {
public:
    static ErrorManager *getErrorManager();
    static void ERROR(int errorType);

    void error(int errorType);

    Error getMostCriticalError();

private:
    ErrorManager();

    Error *mostCriticalError;
};


//////////////////////////// LIST OF ERRORS //////////////////////////////

#define BLANK_ERROR -1

// CRITICAL ERROR... We are screwed............
#define STATE_MACHINE_PTHREAD_FAILED_TO_INIT 1

// Non-critical error, probably somebody made a mistake coding and told it to start twice.
#define STATE_MACHINE_ALREADY_STARTED 2

// critical error, should go to safe mode.
#define STATE_MACHINE_STATE_NOT_A_STATE 3






// Non-critical error, Failed to open file descriptor to gyro
#define GYRO_FD_FAILED_TO_OPEN 7

// Non-critical error, Failed to set gyro slave address correctly
#define GYRO_I2C_ADDRESS_FAILURE 8

// Non-critical error, gryo returned wrong id for check.
#define GYRO_FAILED_COMM_TEST 9

//Non-critical error, MagneTorquer float value out of range
#define MAGNETORQUE_FLOAT_OUT_OF_RANGE 10

// non-critcal error probably?, A config file had a problem loading.
#define ERROR_READING_CONFIG_FILE 11

// non-critcal error, the config file variable was not found.
#define UNABLE_TO_FIND_VARIABLE_IN_CONFIG_FILE 12

/////////////////////////////////// Config errors

// all config file non-critcal errors
#define CONFIG_FILE_READ_DOUBLE_INVALID_VALUE 13
#define CONFIG_FILE_READ_DOUBLE_OUT_OF_RANGE 14

#define CONFIG_FILE_READ_FLOAT_INVALID_VALUE 15
#define CONFIG_FILE_READ_FLOAT_OUT_OF_RANGE 16

#define CONFIG_FILE_READ_INT_INVALID_VALUE 17
#define CONFIG_FILE_READ_INT_OUT_OF_RANGE 18
// completly non-critcal error
#define CONFIG_FILE_SAVE_FOUND_BAD_LINE_IGNORING 19
#define CONFIG_FILE_FAILED_TO_RENAME_FILE 20

// non-critcal error.
#define ADACS_ADC_FAILED_VOLTAGE_READ 25
#define ADACS_ADC_FAILED_TEST 26

// non-critical error completly.
#define COMM_DATA_RECEIVED_BAD_PACKET 27

// critical error - Detector may be off...
#define BIB_GPIO_FAILED_TO_INIT_CORRECTLY 28
// critical error - (either programming error, or single event upset)
#define BIB_GPIO_INVALID_PIN 29
// critical error - detector or beaglebone may not be controlable.
#define BIB_GPIO_FAILED_TO_WRITE_PIN 30
// non-critical error
#define BIB_GPIO_READ_FAILED 31

// non-critical error
#define INA219_FAILED_TO_INIT 32
#define INA219_FAILED_TO_READ 33
#define INA219_FAILED_TEST 34



// Non-critical error
#define TCA9539_FAILED_WRITING_TO_CONFIG_REGS 39
#define TCA9539_INVALID_PIN_NUMBER_GIVEN 40
#define TCA9539_FAILED_READ_FROM_INPUT_REG 41
#define TCA9539_FAILED_WRITE_FROM_INPUT_REG 42

// Unknown critical-ness
#define PIB_GPIO_BURN_WIRE_BAD_PANEL 43
#define PIB_GPIO_BURN_WIRE_FAILED_TO_TURN_ON 44
// +x = 0, -x = 1, +y = 2, -y = 3
#define PIB_GPIO_BURN_WIRE_FAILED_TO_TURN_OFF 48
// +x = 0, -x = 1, +y = 2, -y = 3
// NOTE: Do not use 44,45,46,47,48,49,50,51

//////////////////////////////// EPS temp sensor error

// non-critical error.
#define EPS_TEMP_SENSOR_FAILED_TO_TURN_ON 52
#define EPS_TEMP_SENSOR_FAILED_TO_TURN_OFF 53
#define EPS_TEMP_SENSOR_FAILED_TO_READ_TEMP 54
#define EPS_TEMP_SENSOR_FAILED_COMM_TEST 55
#define EPS_TEMP_SENSOR_FAILED_INIT_COMM 56


/////////////////////////////// Comm data error

// Non-critical error
#define COMM_DATA_FAILED_TO_OPEN_RECEIVE_TEMP_FILE 100
#define COMM_DATA_FAILED_TO_OPEN_RECEIVE_TEMP_READING 101
#define COMM_DATA_FAILED_READ_TEMP_FILE 102
#define COMM_DATA_FAILED_READ_RECEIVE_TEMP_FILE 103
#define COMM_DATA_FAILED_READ_SEND_TEMP_FILE 104

// Non-critical error
#define COMM_DATA_FAILED_TO_CREATE_SEND_TEMP_FILE 120

#define COMM_SEND_COMMAND_BAD_PARAMS 150
#define UHF_MORE_BYTES_THAN_DESIRED 151
#define RECEIVED_BAD_SALT_UHF 152

// critical error.
#define INTREPID_PROCESS_FAILED_TO_INIT 200

/////////////////////////////// Detector errors

// Non-critical error
#define DETECTOR_TEMP_SENSOR_INIT_FAILURE 400
#define DETECTOR_TEMP_SENSOR_READ_FAILURE 401

#define DETECTOR_BIAS_DAC_SET_PREG_FAILURE    402
#define DETECTOR_BIAS_DAC_UPDATE_DREG_FAILURE 403

#define DETECTOR_BIAS_ADC_INIT_FAILURE 404
#define DETECTOR_BIAS_ADC_READ_FAILURE 405

#define DETECTOR_DRS_APP_FAILED_TO_FORK 406




//////////////////////////////// GPIO errors

// Non critical errors.
#define GPIO_EARLIER_FAILURE 2000
#define GPIO_SET_OUTPUT_FAILURE 2001
#define GPIO_DIRECTION_SET_FAILURE 2002
#define GPIO_READ_FAILURE 2003
#define GPIO_SET_AS_INPUT_ONLY 2004
#define GPIO_FREE_FAILURE 2005

////////////////////////////// UART Communication errors

#define INTREPID_UART_SEND_COMMAND_BAD_PARAMS 5000
#define INTREPID_UART_FAILED_TO_SEND 5001

// non-critical
#define INTREPID_UART_RECEIVED_NO_FILE_ON_BEAGLEBONE 5010
#define INTREPID_UART_RECEIVED_BAD_COMMAND_ON_PACKET_REQUEST 5011

#define BBB_UART_RECEIVED_LENGTH_LARGER_THAN_MAX 5020
#define BBB_UART_RECEIVED_LENGTH_LESS_THAN_MIN 5021



/////////////////////////////// UART hardware errors
// Non-critical error, Intrepid file descriptor for UART did not open.
#define INTREPID_UART_FILE_DESCRIPTOR_FAILED_TO_OPEN 5200
// Non-critical error, Failed to set UART attributes on Intrepid
#define INTREPID_UART_ATTRIBUTES_NOT_SET 5201
// Non-critical error, Failed to write data to BBB from Intrepid UART
#define INTREPID_UART_WRITE_FAILED 5201
#define INCA_UART_IMPOSSIBLE_TYPE 5202


#define BEAGLEBONE_FAILED_TO_TURN_ON_RUN_FUNCTION 5300



////////////////////////////////// Boot errors
// I am not sure the correct action to take in this case.
// Non-critical, since the state machine hasn't started yet.
#define BOOT_CONFIG_FILE_FAILED_TO_SAVE 5500
// Non-critical, handled by setting deploy time to 45 minutes
#define BOOT_CONFIG_FILE_FAILED_TO_READ_DEPLOY_TIME 5501
#define BOOT_CONFIG_FILE_FAILED_TO_SAVE_FIRST_BOOT_TIME 5502
#define BOOT_CONFIG_FILE_FAILED_TO_READ_FIRST_BOOT_TIME 5503


#define ANTENNA1_GPIO_NOT_TURNING_ON 5510
#define ANTENNA2_GPIO_NOT_TURNING_ON 5511
#define ANTENNA1_GPIO_NOT_TURNING_OFF 5512
#define ANTENNA2_GPIO_NOT_TURNING_OFF 5513





#endif /* ErrorManager_hpp */
