/**********************************************************************************
 * [FILE NAME]: HMI_ECU.c
 *
 * [AUTHOR]: Ahmed Ezz El-Deen Mahmoud
 *
 * [DATE CREATED]: April 4, 2021
 *
 * [Description]: The main source file for the HMI_ECU
 ***********************************************************************************/

#include "HMI_ECU.h"

int main()
{
	LCD_init();

	UART_init(&UART_configuration);

	LCD_clearScreen();
	LCD_displayStringRowColumn(ROW_ZERO,COLUMN_EIGHT,"DOOR LOCKER SYSTEM");
	LCD_displayStringRowColumn(ROW_ONE,COLUMN_THIRTEEN,"WELCOME");
	_delay_ms(FOUR_SECONDS_DELAY);

	ifPassSavedORNOT();   /* to check either it's first time for the user to log in or no */

	SREG |= (1<<7);       /* Enable I-Bit for Interrupts */

	while(1){
		(*ptr_states[g_stateNum])();   /* calling functions from the array of functions */
	}

}

/***************************************************************************************************
 * [Function Name]:   createandStorePassword
 *
 * [Description]:     Function to Create and Store password in a global array
 *
 * [Arguments]:       VOID
 *
 * [Returns]:         VOID
 ***************************************************************************************************/
void createandStorePassword(void)
{
	/*
	 * 1. DISPLAY ON SCREEN A MESSAGE TO ENTER THE PASSWORD
	 * 2. ENTER IT AGAIN AND CHECK IF IT MATCHES
	 * 3. START FILLING IN THE PASSWORD FROM THE USER TO A GLOBAL ARRAY
	 * 4. SEND COMMAND TO CONTROL_ECU (PASSWORD_SEND)
	 * 5. SENDING THE PASSWORD TO THE CONTROL_ECU AND IF IT MATCHES STORE IT
	 * 6. IF IT DOESN'T MATCH DISPLAY A MESSAGE ON SCREEN
	 */
	if(g_flag==0){
		LCD_clearScreen();
		LCD_displayStringRowColumn(ROW_ZERO,COLUMN_ZERO,"PLEASE ENTER NEW PASSWORD(5):");
		LCD_goToRowColumn(ROW_ONE,COLUMN_TWELVE);
		fillInPassword(g_password);
		sendCommand(PASSWORD_SEND);
		SendPassword(g_password);
		g_flag=1;
	}
	if(g_flag==1)
	{
		LCD_clearScreen();
		LCD_displayStringRowColumn(ROW_ZERO,COLUMN_ZERO,"PLEASE RE-ENTER PASSWORD(5):");
		LCD_goToRowColumn(ROW_ONE,COLUMN_TWELVE);
		fillInPassword(g_password);
		sendCommand(PASSWORD_CONFIRMATION_SEND);
		SendPassword(g_password);
		g_flag=2;
	}
	if (g_flag==2)
	{
		switch(recieveCommand())
		{
		case PASSWORD_MATCH:
			g_flag=3;
			g_stateNum=1;
			break;

		case PASSWORD_NOT_MATCHED:
			PasswordDONTMATCHScreen();
			g_flag=0;
			break;
		}
	}
}

/***************************************************************************************************
 * [Function Name]:   SendPassword
 *
 * [Description]:     Function to Send password that is stored in the global array
 *
 * [Arguments]:       ARRAY of unsigned integers and it's size is 5 (password size)
 *
 * [Returns]:         VOID
 ***************************************************************************************************/
void SendPassword(uint8 a_arr[])
{
	/*
	 * 1. SEND A READY COMMAND TO THE CONTROL_ECU TO SAY THAT HMI_ECI IS READY
	 * 2.WAITING UNTIL THE CONTROL_ECU SEND A READY COMMAND TOO
	 * 3.SENDING THE PASSWORD ENTERED BY THE USER FROM THE GLOBAL ARRAY
	 * 4. WAITING UNTIL THE CONTROL_ECU SENDS DONE COMMAND
	 */
	UART_sendByte(READY);
	while(UART_recieveByte() != READY){};

	for(uint8 i=0;i<PASSWORD_ARRAY_SIZE;i++)
	{
		UART_sendByte(a_arr[i]);
	}
	while(UART_recieveByte() != DONE){};
}
/***************************************************************************************************
 * [Function Name]:   sendCommand
 *
 * [Description]:     Function to Send commands to the control ECU Using USART
 *
 * [Arguments]:       unsigned integer command number
 *
 * [Returns]:         VOID
 ***************************************************************************************************/
void sendCommand(uint8 command)
{
	/*
	 * 1. SEND A READY COMMAND TO THE CONTROL_ECU TO SAY THAT HMI_ECI IS READY
	 * 2.WAITING UNTIL THE CONTROL_ECU SEND A READY COMMAND TOO
	 * 3.SENDING A COMMAND FROM THE HMI_ECU
	 * 4. WAITING UNTIL THE CONTROL_ECU SENDS DONE COMMAND
	 */
	UART_sendByte(READY);
	while(UART_recieveByte() != READY){};
	UART_sendByte(command);
	while(UART_recieveByte() != DONE){};

}
/***************************************************************************************************
 * [Function Name]:   recieveCommand
 *
 * [Description]:     Function to Recieve commands from the control ECU Using USART
 *
 * [Arguments]:       VOID
 *
 * [Returns]:         unsigned integer command number
 ***************************************************************************************************/
uint8 recieveCommand(void)
{
	/*
	 * 1.WAITING UNTIL THE CONTROL_ECU SEND A READY COMMAND
	 * 2. SEND A READY COMMAND TO THE CONTROL_ECU TO SAY THAT HMI_ECI IS READY
	 * 3. RECIEVE A COMMAND FROM THE CONTROL_ECU
	 * 4. SENDING TO THE  CONTROL_ECU  DONE COMMAND
	 * 5. RETURN THE COMMAND SENT FROM THE COTROL_ECU
	 */
	while(UART_recieveByte() != READY){};
	UART_sendByte(READY);
	command=UART_recieveByte();
	UART_sendByte(DONE);
	return command;
}

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
void mainFucntion(void)
{
	/*
	 * 1. DISPLAY THE MAIN OPTIONS TO THE USER ON THE SCREEN
	 * 2.DEPENDING ON WHICH OPTION HE CHOSE DO IT
	 * 3.CASE'+':
	 *           REQUEST THE OLD PASSOWRD
	 *           FILL IN THE PASSWORD FROM THE USER
	 *           SEND COMMAND TO CONTROL_ECU TO CHECK IF PASSWORD MATCHES WHAT THE USER ENTERED OR NOT
	 *           IF IT MATCHES:
	 *                         GO TO STATE NUMBER 0 TO CHANGE THE PASSWORD
	 *           IF NOT:
	 *                  DISPLAY PASSWORD WRONG MESSAGE ALSO INCREMENT THE GLOBAL WRONG VARIABLE
	 *   CASE'-':
	 *            REQUEST THE OLD PASSOWRD
	 *            FILL IN THE PASSWORD FROM THE USER
	 *            SEND COMMAND TO CONTROL_ECU TO CHECK IF PASSWORD MATCHES WHAT THE USER ENTERED OR NOT
	 *            IF IT MATCHES:
	 *                          GO TO STATE NUMBER 2 TO OPEN THE DOOR
	 *            IF NOT:
	 *                   DISPLAY PASSWORD WRONG MESSAGE ALSO INCREMENT THE GLOBAL WRONG VARIABLE
	 */
	LCD_clearScreen();
	LCD_displayStringRowColumn(ROW_ZERO,COLUMN_SEVEN,"(+): CHANGE PASSWORD");
	LCD_displayStringRowColumn(ROW_ONE,COLUMN_SEVEN,"(-): OPEN DOOR");
	switch(KeyPad_getPressedKey())
	{
	case '+':
		LCD_clearScreen();
		LCD_displayStringRowColumn(ROW_ZERO,COLUMN_ZERO,"PLEASE ENTER OLD PASSWORD:(5)");
		LCD_goToRowColumn(ROW_ONE,COLUMN_TWELVE);
		_delay_ms(ONE_AND_HALF_SEC_DELAY);
		fillInPassword(g_password);
		sendCommand(CHECK_PASSWORD);
		SendPassword(g_password);
		switch (recieveCommand())
		{
		case PASSWORD_MATCH:
			g_flag=0;
			g_stateNum=0;
			g_wrong=0;
			break;
		case PASSWORD_NOT_MATCHED:
			wrongPasswordScreen();
			break;
		}
		break;
		case '-':
			LCD_clearScreen();
			LCD_displayStringRowColumn(ROW_ZERO,COLUMN_ZERO,"PLEASE ENTER OLD PASSWORD:(5)");
			LCD_goToRowColumn(ROW_ONE,COLUMN_TWELVE);
			_delay_ms(ONE_AND_HALF_SEC_DELAY);
			fillInPassword(g_password);
			sendCommand(CHECK_PASSWORD);
			SendPassword(g_password);
			switch (recieveCommand()){
			case PASSWORD_MATCH:
				sendCommand(OPEN_DOOR);
				g_stateNum=2;
				g_wrong=0;
				break;
			case PASSWORD_NOT_MATCHED:
				wrongPasswordScreen();
				break;
			}
			break;
	}

}
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
void ifPassSavedORNOT(void)
{
	/*
	 * 1. SEND A COMMAND TO CONTROL_ECU TO CHECK IF IT HAS A SAVED PASSWORD IN EEPROM OR NOT YET
	 * 2.WAITING UNTIL THE CONTROL_ECU SEND COMMAND TOO
	 * IF YES_SAVED COMMAD:
	 *                    GO TO STATE 1 FOR THE MAIN FUNCTION
	 * IF NO_SAVED_PASSWORD:
	 *                     GO TO STATE 0 TO ENTER A NEW PASSWORD
	 */
	sendCommand(CHECK_IF_SAVED);
	switch (recieveCommand())
	{
	case YES_SAVED:
		g_stateNum=1;
		break;
	case NO_SAVED_PASSWORD:
		g_stateNum=0;
		break;

	}

}

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
void wrongPasswordScreen(void)
{
	/*
	 *1. DISPLAY A WRONG PASSWORD MESSAGE ON SCREEN
	 *2. INCREMENT THE GLOBAL WRONG VARIABLE
	 *3. SEND COMMANT TO THE CONTROL_ECU THAT THE USER HAS ENTERED A WRONG PASSWORD
	 *4. IF PASSWORD ENTERED IS WRONG 3 TIMES IN A ROW
	 *   IT WILL DISPLAY AN ALARM MESSAGE ON THE SCREEN
	 */
	LCD_clearScreen();
	LCD_displayStringRowColumn(ROW_ZERO,COLUMN_NINE,"WRONG PASSWORD!!");
	g_wrong++;
	sendCommand(WRONG_PASSWORD);
	if(g_wrong == MAX_WRONG_COUNTER)
	{
		ALERTscreen();
	}
	_delay_ms(FOUR_SECONDS_DELAY);

}
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
void fillInPassword(uint8 a_arr[])
{
	uint8 counter=0;
	g_key=KeyPad_getPressedKey();
	while(g_key != '=')
	{
		g_key=KeyPad_getPressedKey();
		if( ( g_key != '+' && g_key != '-' && g_key != '*' && g_key != '%' && g_key != '=') || (g_key>= 0 && g_key<= 9) )
		{
			if(counter < PASSWORD_ARRAY_SIZE)
			{
				a_arr[counter]=g_key;
				LCD_displayCharacter('*');
				counter++;
			}
		}
		_delay_ms(THREE_SECONDS_DELAY);
	}

}

/***************************************************************************************************
 * [Function Name]:   PasswordDONTMATCHScreen
 *
 * [Description]:     Function to display a message that the password you entered don't match with each other
 *
 * [Arguments]:       VOID
 *
 * [Returns]:         VOID
 ***************************************************************************************************/

void PasswordDONTMATCHScreen(void)
{
	LCD_clearScreen();
	LCD_displayStringRowColumn(ROW_ZERO,COLUMN_ZERO,"PASSWORD DON'T MATCH..");
	_delay_ms(FOUR_SECONDS_DELAY);

}
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
void ALERTscreen(void)
{
	/*
	 * 1.INITIATE TIMER1 WITH THE GIVEN CONFIGURATIONS
	 * 2.SET THE CALL BACK FUNCTION
	 * 3. WHILE TIMER HASN'T FINISHED COUNTING DISPLAY ALERT MESSAGE
	 * 4. IF TIME REACHED THE REQUIRED COUNTS
	 *    WRONG COUNTER BACK TO ZERO COUNTS ALSO GO BACK TO THE MAIN FUNCTION
	 */
	LCD_clearScreen();
	Timer_init(&TIMER_configuration);
	Timer_setCallBack(g_tickCounterAlarm,Timer1);

	while(g_tick != TIMER_TICKS_1MINUTE)
	{
		LCD_displayStringRowColumn(ROW_ZERO,COLUMN_TWELVE,"ALERT!!!!");
		LCD_displayStringRowColumn(ROW_ONE,COLUMN_TWELVE,"THEIF!!!!");
	}
	if(g_tick==TIMER_TICKS_1MINUTE_STOP)
	{
		g_wrong=0;
		g_stateNum=1;
	}

}
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
void doorIsOpeningCloseingScreen(void)
{
	/*
	 * 1.INITIATE TIMER1 WITH THE GIVEN CONFIGURATIONS
	 * 2.SET THE CALL BACK FUNCTION
	 * 3. WHILE TIMER HASN'T FINISHED COUNTING DISPLAY DOOR IS UNLOCKING AND LOCKING  MESSAGE
	 * 4. IF TIME REACHED THE REQUIRED COUNTS
	 *    WRONG COUNTER BACK TO ZERO COUNTS ALSO GO BACK TO THE MAIN FUNCTION
	 */
	LCD_clearScreen();
	Timer_init(&TIMER_configuration);
	Timer_setCallBack(g_tickCounterMotor,Timer1);
	while(g_tick != TIMER_TICKS_15SEC)
	{
		LCD_displayStringRowColumn(ROW_ZERO,COLUMN_SEVEN,"DOOR IS UNLOCKING..");
	}
	if(g_tick == TIMER_TICKS_15SEC)
	{
		LCD_clearScreen();
		_delay_ms(FOUR_SECONDS_DELAY);
	}
	while(g_tick>= TIMER_TICKS_15SEC && g_tick < TIMER_TICKS_15SEC_STOP)
	{
		if(g_tick == TIMER_TICKS_15SEC_NEXT_STAGE)
		{
			g_stateNum=1;
		}
		LCD_displayStringRowColumn(0,7,"DOOR IS LOCKING..");
	}

}
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
void g_tickCounterMotor(void)
{
	g_tick++;

	if(g_tick==TIMER_TICKS_15SEC_STOP)
	{
		Timer_DeInit(Timer1);
		g_tick=0;
	}
}
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
void g_tickCounterAlarm(void)
{
	g_tick++;
	if(g_tick == TIMER_TICKS_1MINUTE_STOP)
	{
		Timer_DeInit(Timer1);
		g_tick=0;

	}
}





