/**********************************************************************************
 * [FILE NAME]: ALARM.c
 *
 * [AUTHOR]: Ahmed Ezz El-Deen Mahmoud
 *
 * [DATE CREATED]: April 4, 2021
 *
 * [Description]: The ALARM source file for the CONTROL_ECU
 ***********************************************************************************/
#include "ALARM.h"

/********************************************************************************
 * [Function Name]:      ALARM_INIT
 *
 * [Description]:        Function to init the ALARM
 *
 * [Arguments]:          VOID
 *
 * [Returns]:            VOID
 *******************************************************************************/
void ALARM_INIT(void)
{
	BUZZER_AND_LED_DIRECTION_PORT |=  (1<<BUZZER_PIN) | (1<<RED_LED_PIN);
	BUZZER_AND_LED_DATA_PORT |= 0xF3;

}

/********************************************************************************
 * [Function Name]:         ALARM_ON
 *
 * [Description]:           Function to turn ON the ALARM
 *
 * [Arguments]:             VOID
 *
 * [Returns]:               VOID
 *******************************************************************************/
void ALARM_ON(void)
{
	TOGGLE_BIT(BUZZER_AND_LED_DATA_PORT , BUZZER_PIN );

	TOGGLE_BIT(BUZZER_AND_LED_DATA_PORT , RED_LED_PIN );

	_delay_ms(500);

}
/********************************************************************************
 * [Function Name]:         ALARM_OFF
 *
 * [Description]:           Function to turn OFF the ALARM
 *
 * [Arguments]:             VOID
 *
 * [Returns]:               VOID
 *******************************************************************************/
void ALARM_OFF(void)
{
	CLEAR_BIT(BUZZER_AND_LED_DATA_PORT , BUZZER_PIN);

	CLEAR_BIT(BUZZER_AND_LED_DATA_PORT , RED_LED_PIN );

}
