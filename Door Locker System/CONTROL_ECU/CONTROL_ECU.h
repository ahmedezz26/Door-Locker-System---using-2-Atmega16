/**********************************************************************************
 * [FILE NAME]: CONTROL_ECU.h
 *
 * [AUTHOR]: Ahmed Ezz El-Deen Mahmoud
 *
 * [DATE CREATED]: April 4, 2021
 *
 * [Description]: The main header file for the CONTROL_ECU
 * contents:
 *          DRIVERS USED IN THE CONTROL_ECU
 *          DEFINED COMMANDS
 *          UART & TIMER CONFIGURATIONS
 *          GLOBALE VARIABLES
 *          FUNCTIONS
 ***********************************************************************************/

#ifndef CONTROL_ECU_H_
#define CONTROL_ECU_H_

/************************************************************************************************
 *                                       CONTROL_ECU                                            *
 ************************************************************************************************/


/************************************************************************************************
 *                                     DRIVERS INCLUDES                                         *
 ************************************************************************************************/

#include"external_eeprom.h"
#include"micro_config.h"
#include"std_types.h"
#include"UART.h"
#include"common_macros.h"
#include"Motor.h"
#include"TIMER.h"
#include"ALARM.h"


/************************************************************************************************
 *                                    DEFINED COMMANDS                                          *
 ************************************************************************************************/

#define READY                                              0x01
#define DONE                                               0x02
#define PASSWORD_SEND                                      0x03
#define PASSWORD_MATCH                                     0x04
#define PASSWORD_NOT_MATCHED                               0x05
#define PASSWORD_CONFIRMATION_SEND                         0x06
#define CHANGE_PASSWORD                                    0x07
#define OPEN_DOOR                                          0x08
#define CHECK_PASSWORD                                     0x09
#define WRONG_PASSWORD                                      0xA
#define CHECK_IF_SAVED                                      0xB
#define YES_SAVED                                           0xC
#define NO_SAVED_PASSWORD                                   0xD
#define TICKS_15SECS                                        2
#define TICKS_15SECS_REVERSE                                3
#define TIMER_TICKS_15SEC_STOP                              4
#define TIMER_TICKS_1MINUTE                                 12
#define TIMER_TICKS_1MINUTE_STOP                            13
#define FOUR_SECONDS_DELAY                                  4000
#define HALF_SECOND_DELAY                                   500
#define MAX_WRONG_COUNTER                                   3
#define PASSWORD_ARRAY_SIZE                                 5
#define DEFUALT_VALUE_OF_EEPROM                             1

/************************************************************************************************
 *                                 UART & TIMER CONFIGURATIONS                                  *
 ************************************************************************************************/

UART_config UART_configuration = {ASYNCHRONOUS,DISABLED,ONE,EIGHT};
Timer_Config TIMER_configuration= {0,39062,Timer1,F_CPU_1024,Compare};


/************************************************************************************************
 *                                GLOBAL VARIABLES                                              *
 ************************************************************************************************/

uint8 g_password[5];
uint8 g_passmatch[5];
uint8 savedpass[5];
uint8 command;
uint8 g_tick=0;
uint8 g_wrong=0;

/************************************************************************************************
 *                                   FUNCTIONS                                                  *
 ************************************************************************************************/

/***************************************************************************************************
 * [Function Name]:   recievePassword
 *
 * [Description]:     Function to recieve password that is stored in the global array
 *
 * [Arguments]:       ARRAY of unsigned integers and it's size is 5 (password size)
 *
 * [Returns]:         VOID
 ***************************************************************************************************/
void recievePassword(uint8 a_arr[]);

/***************************************************************************************************
 * [Function Name]:   storePasswordInMemory
 *
 * [Description]:     Function to  store password in the External EEPROM
 *
 * [Arguments]:       VOID
 *
 * [Returns]:         VOID
 ***************************************************************************************************/
void storePasswordInMemory(void);

/***************************************************************************************************
 * [Function Name]:   Match_or_NoMatch
 *
 * [Description]:     Function to recieve password that is stored in the global array
 *
 * [Arguments]:       TWO ARRAYS of unsigned integers and it's size is 5 (password size)
 *
 * [Returns]:         BOOLEAN TRUE OR FALSE
 ***************************************************************************************************/
bool Match_or_NoMatch(uint8 a_arr1[],uint8 a_arr2[]);

/***************************************************************************************************
 * [Function Name]:   recieveCommand
 *
 * [Description]:     Function to Recieve commands from the HMI ECU Using USART
 *
 * [Arguments]:       VOID
 *
 * [Returns]:         unsigned integer command number
 ***************************************************************************************************/
uint8 recieveCommand(void);

/***************************************************************************************************
 * [Function Name]:   sendCommand
 *
 * [Description]:     Function to Send commands to the hmi ECU Using USART
 *
 * [Arguments]:       unsigned integer command number
 *
 * [Returns]:         VOID
 ***************************************************************************************************/
void sendCommand(uint8 command);

/***************************************************************************************************
 * [Function Name]:   getSavedPassword
 *
 * [Description]:     Function to get the saved password in the EEPROM and store it in a global array
 *
 * [Arguments]:       Array of unsigned integers of size 5 (password size)
 *
 * [Returns]:         VOID
 ***************************************************************************************************/
void getSavedPassword(uint8 a_arr[]);


/***************************************************************************************************
 * [Function Name]:   g_tickCounterMotor
 *
 * [Description]:     Function to assign the call back function(every interrupt) to of
 *                    the motor timer (15 seconds)
 *
 * [Arguments]:       VOID
 *
 * [Returns]:         VOID
 ***************************************************************************************************/
void g_tickCounterMotor(void);

/***************************************************************************************************
 * [Function Name]:   g_tickCounterAlarm
 *
 * [Description]:     Function to assign the call back function(every interrupt) to of
 *                    the Alarm timer (1 minute)
 *
 * [Arguments]:       VOID
 *
 * [Returns]:         VOID
 ***************************************************************************************************/
void g_tickCounterAlarm(void);

#endif /* CONTROL_ECU_H_ */
