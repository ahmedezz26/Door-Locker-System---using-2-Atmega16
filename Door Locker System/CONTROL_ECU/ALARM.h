/**********************************************************************************
 * [FILE NAME]: ALARM.h
 *
 * [AUTHOR]: Ahmed Ezz El-Deen Mahmoud
 *
 * [DATE CREATED]: April 4, 2021
 *
 * [Description]: The ALARM header file for the CONTROL_ECU
 ***********************************************************************************/

#ifndef ALARM_H_
#define ALARM_H_

#include "std_types.h"
#include "micro_config.h"
#include "common_macros.h"

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

#define BUZZER_AND_LED_DIRECTION_PORT                 DDRD
#define BUZZER_PIN                                    PD2
#define RED_LED_PIN                                   PD3
#define BUZZER_AND_LED_DATA_PORT                      PORTD


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/********************************************************************************
 * [Function Name]:      ALARM_INIT
 *
 * [Description]:        Function to init the ALARM
 *
 * [Arguments]:          VOID
 *
 * [Returns]:            VOID
 *******************************************************************************/
void ALARM_INIT(void);


/********************************************************************************
 * [Function Name]:         ALARM_ON
 *
 * [Description]:           Function to turn ON the ALARM
 *
 * [Arguments]:             VOID
 *
 * [Returns]:               VOID
 *******************************************************************************/
void ALARM_ON(void);

/********************************************************************************
 * [Function Name]:         ALARM_OFF
 *
 * [Description]:           Function to turn OFF the ALARM
 *
 * [Arguments]:             VOID
 *
 * [Returns]:               VOID
 *******************************************************************************/
void ALARM_OFF(void);




#endif /* ALARM_H_ */

