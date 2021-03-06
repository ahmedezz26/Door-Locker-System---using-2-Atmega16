/**********************************************************************************
 * [FILE NAME]: HMI_ECU.h
 *
 * [AUTHOR]: Ahmed Ezz El-Deen Mahmoud
 *
 * [DATE CREATED]: April 4, 2021
 *
 * [Description]: The main header file for the HMI_ECU
 * contents:
 *          DRIVERS USED IN THE HMI_ECU
 *          DEFINED COMMANDS
 *          UART & TIMER CONFIGURATIONS
 *          GLOBALE VARIABLES
 *          FUNCTIONS
 *          ARRAY OF POINTER TO FUNCTIONS THAT TAKE VOID AS AN ARGUMENT AND RETURN VOID
 ***********************************************************************************/

#ifndef HMI_ECU_H_
#define HMI_ECU_H_

/************************************************************************************************
 *                                       HMI_ECU                                                *
 ************************************************************************************************/


/************************************************************************************************
 *                                     DRIVERS INCLUDES                                         *
 ************************************************************************************************/
#include"LCD.h"
#include"keypad.h"
#include"UART.h"
#include"common_macros.h"
#include"std_types.h"
#include"micro_config.h"
#include"TIMER.h"

/************************************************************************************************
 *                                    DEFINED COMMANDS                                          *
 ************************************************************************************************/

#define READY                                               0x01
#define DONE                                                0x02
#define PASSWORD_SEND                                       0x03
#define PASSWORD_MATCH                                      0x04
#define PASSWORD_NOT_MATCHED                                0x05
#define PASSWORD_CONFIRMATION_SEND                          0x06
#define CHANGE_PASSWORD                                     0x07
#define OPEN_DOOR                                           0x08
#define CHECK_PASSWORD                                      0x09
#define WRONG_PASSWORD                                      0xA
#define CHECK_IF_SAVED                                      0xB
#define YES_SAVED                                           0xC
#define NO_SAVED_PASSWORD                                   0xD
#define PASSWORD_ARRAY_SIZE                                 5
#define ONE_AND_HALF_SEC_DELAY                              1500
#define THREE_SECONDS_DELAY                                 3000
#define FOUR_SECONDS_DELAY                                  4000
#define ROW_ZERO                                            0
#define ROW_ONE                                             1
#define COLUMN_ZERO                                         0
#define COLUMN_SEVEN                                        7
#define COLUMN_EIGHT                                        8
#define COLUMN_NINE                                         9
#define COLUMN_TWELVE                                       12
#define COLUMN_THIRTEEN                                     13
#define TIMER_TICKS_15SEC                                   2
#define TIMER_TICKS_15SEC_STOP                              4
#define TIMER_TICKS_15SEC_NEXT_STAGE                        3
#define TIMER_TICKS_1MINUTE                                 12
#define TIMER_TICKS_1MINUTE_STOP                            13
#define GLOBALE_ARRAY_OF_POINTERS_SIZE                      3
#define MAX_WRONG_COUNTER                                   3
/************************************************************************************************
 *                                 UART & TIMER CONFIGURATIONS                                  *
 ************************************************************************************************/
/* ASYNCHRONOUS: UART_mode
 * DISABLED: UART_parityBit
 * ONE : UART_stopBit
 * EIGHT : UART_characterSize
 */
UART_config UART_configuration = {ASYNCHRONOUS,DISABLED,ONE,EIGHT};

/* 0: timer_InitialValue
 * 39062: timer_compare_MatchValue
 * Timer1 : timer_ID
 * F_CPU_1024 : timer_clock
 * Compare : timer_mode
 */

Timer_Config TIMER_configuration= {0,39062,Timer1,F_CPU_1024,Compare};

/************************************************************************************************
 *                                GLOBAL VARIABLES                                              *
 ************************************************************************************************/
static volatile uint8 g_flag=0;               /*global flag variable to check on */
static volatile uint8  g_stateNum;            /*global state number of the main function */
uint8 g_key;                                  /*global variable to hold the key value */
uint8 g_password[5];                          /*global array to store the password in it */
uint8 command;                                /*global variable to store the commands in it */
uint8 g_wrong=0;                              /*global variable to count wrong password entered times*/
uint8 g_tick=0;                               /*global ticks to count timer interrupts*/

/************************************************************************************************
 *                                   FUNCTIONS                                                  *
 ************************************************************************************************/

/***************************************************************************************************
 * [Function Name]:   createandStorePassword
 *
 * [Description]:     Function to Create and Store password in a global array
 *
 * [Arguments]:       VOID
 *
 * [Returns]:         VOID
 ***************************************************************************************************/
void createandStorePassword(void);


/***************************************************************************************************
 * [Function Name]:   SendPassword
 *
 * [Description]:     Function to Send password that is stored in the global array
 *
 * [Arguments]:       ARRAY of unsigned integers and it's size is 5 (password size)
 *
 * [Returns]:         VOID
 ***************************************************************************************************/
void SendPassword(uint8 a_arr[]);


/***************************************************************************************************
 * [Function Name]:   sendCommand
 *
 * [Description]:     Function to Send commands to the control ECU Using USART
 *
 * [Arguments]:       unsigned integer command number
 *
 * [Returns]:         VOID
 ***************************************************************************************************/
void sendCommand(uint8 command);

/***************************************************************************************************
 * [Function Name]:   recieveCommand
 *
 * [Description]:     Function to Recieve commands from the control ECU Using USART
 *
 * [Arguments]:       VOID
 *
 * [Returns]:         unsigned integer command number
 ***************************************************************************************************/
uint8 recieveCommand(void);

/***************************************************************************************************
 * [Function Name]:   mainFucntion
 *
 * [Description]:     The main functions of the HMI_ECU that has 2 options either to open the door or
 *                    to change the password and according to the user decision it send the chosen command
 *                    to the control ECU
 *
 * [Arguments]:       VOID
 *
 * [Returns]:         VOID
 ***************************************************************************************************/
void mainFucntion(void);

/***************************************************************************************************
 * [Function Name]:   ifPassSavedORNOT
 *
 * [Description]:     It checks if it's the user had already entered a password safely and saved
 *                    in the EEPROM or it's his first time creating a password
 *
 * [Arguments]:       VOID
 *
 * [Returns]:         VOID
 ***************************************************************************************************/
void ifPassSavedORNOT(void);

/***************************************************************************************************
 * [Function Name]:   wrongPasswordScreen
 *
 * [Description]:     Function used to display a wrong entered password on screen also count
 *                    the number of times that the user entered a wrong password
 *
 * [Arguments]:       VOID
 *
 * [Returns]:         VOID
 ***************************************************************************************************/
void wrongPasswordScreen(void);

/***************************************************************************************************
 * [Function Name]:   fillInPassword
 *
 * [Description]:     Function to Fill in the password from the user using the keypad interfacing
                      and store it in a global array
 *
 * [Arguments]:       ARRAY of unsigned integers and it's size is 5 (password size)
 *
 * [Returns]:         VOID
 ***************************************************************************************************/
void fillInPassword(uint8 a_arr[]);

/***************************************************************************************************
 * [Function Name]:   PasswordDONTMATCHScreen
 *
 * [Description]:     Function to display a message that the password you entered don't match with each other
 *
 * [Arguments]:       VOID
 *
 * [Returns]:         VOID
 ***************************************************************************************************/
void PasswordDONTMATCHScreen(void);

/***************************************************************************************************
 * [Function Name]:   ALERTscreen
 *
 * [Description]:     Function to display a message for 1 minute that the user entered a
 *                    wrong password for 3 times in row
 *
 * [Arguments]:       VOID
 *
 * [Returns]:         VOID
 ***************************************************************************************************/
void ALERTscreen(void);

/***************************************************************************************************
 * [Function Name]:   doorIsOpeningCloseingScreen
 *
 * [Description]:     Function to display a message for 15 seconds that the door is closing
 *                    and the door is opening for another 15 seconds
 *
 * [Arguments]:       VOID
 *
 * [Returns]:         VOID
 ***************************************************************************************************/
void doorIsOpeningCloseingScreen(void);


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



/************************************************************************************************
 *            ARRAY OF POINTER TO FUNCTIONS THAT TAKE VOID AS AN ARGUMENT AND RETURN VOID       *                                       *
 ************************************************************************************************/

void (*ptr_states[GLOBALE_ARRAY_OF_POINTERS_SIZE])(void)={createandStorePassword,mainFucntion,doorIsOpeningCloseingScreen};


#endif /* HMI_ECU_H_ */
