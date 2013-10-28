#include "terminal.h"
#include "adc.h"
#include "nf/nfv2.h"
#include "vt100.h"

extern ADC_St				ADC;
extern NF_STRUCT_ComBuf 	NFComBuf;

//main display function
void Display(void)
{
	static u8 Display = TERM_DISPLAY_NORMAL;
	static u8 LastDisplay = TERM_DISPLAY_NORMAL;
	
//	if(TIM_Sys_Flags.TimeDisp200ms)
//	{
//		TIM_Sys_Flags.TimeDisp200ms = 0;

	if(usart1_check_RX())
	{
		switch(usart1_get_byte())
		{		
			case '0':
				Display = TERM_DISPLAY_NORMAL;
				break;
			case '1':
				Display = TERM_DISPLAY_ANALOG1;
				break;
			case '2':
				Display = TERM_DISPLAY_ANALOG2;
				break;
			case '3':
				Display = TERM_DISPLAY_ANALOG3;
				break;
			case '4':
				Display = TERM_DISPLAY_DIGITAL;
				break;
			case '\r':
				Display = TERM_DISPLAY_MENU;
				break;
		}
	}

	switch(Display)
	{
		case TERM_DISPLAY_NORMAL:
			if (LastDisplay != Display)
			{
				VT_SHOW_CURSOR;
				printf("\n\r Normal view\n\r");
			}
			break;
		case TERM_DISPLAY_ANALOG1:
			if (LastDisplay != Display)
			{
				VT_HIDE_CURSOR;
				printf("\n\r Analog measurements1\n\r\n");
				DisplayAnalog1();
			}
			VT_CURSOR_UP(11);
			DisplayAnalog1();
			break;
	   case TERM_DISPLAY_ANALOG2:
			if (LastDisplay != Display)
			{
				VT_HIDE_CURSOR;
				printf("\n\r Analog measurements2\n\r\n");
				DisplayAnalog2();
			}
			VT_CURSOR_UP(12);
			DisplayAnalog2();
			break;
	   case TERM_DISPLAY_ANALOG3:
			if (LastDisplay != Display)
			{
				VT_HIDE_CURSOR;
				printf("\n\r Analog measurements3\n\r\n");
				DisplayAnalog3();
			}
			VT_CURSOR_UP(4);
			DisplayAnalog3();
			break;
	   case TERM_DISPLAY_DIGITAL:
			if (LastDisplay != Display)
			{
				VT_HIDE_CURSOR;
				printf("\n\r Digital signals\n\r\n");
				DisplayDigital();
			}
			VT_CURSOR_UP(8);
			DisplayDigital();
			break;
	   case TERM_DISPLAY_MENU:
			VT_SHOW_CURSOR;
			Menu();
			Display = TERM_DISPLAY_NORMAL;
			break;
		default:
			break;
	}
	LastDisplay = Display;
}

//******************************************************************************
//******************************** MAIN MENU ***********************************
//******************************************************************************

void Menu(void) 
{  	
	printf("\n\rSERVICE MODE\n\r");
	
	while (1) {	
		printf("\n\r\nMAIN MENU:\n\r\n");		
		printf(" t - Test module\r\n");
		printf(" o - Advanced options\n\r");
        printf(" r - Reset\r\n");
		printf(" x - Back to NORMAL MODE\r\n");
		printf("\nYour choice? ");		
												 
		while(!usart1_check_RX())
			IWDG_ReloadCounter();
		
		printf("\n\r\n");
		
		switch(usart1_get_byte()) 
		{
			case 't' : Menu_TestModule(); break;        
			case 'o' : Menu_Advanced(); break;
			case 'r' : Menu_Reset(); break;
			case 'x' : Menu_Exit(); return; break;
			default  : Menu_UnknownCommand(); break;
		}
	}
}

//******************************************************************************
//*************************** TEST MODULE MENU *********************************
//******************************************************************************

void Menu_TestModule(void) 
{
    while(1) 
	{	
		printf("\n\r\nTEST MODULE MENU:\n\r\n");
        printf(" 1 - Test serwo\r\n");
        printf(" 2 - Test zaw2\r\n");
        printf(" s - SDmicro card terminal\r\n");
		printf(" x - Back to MAIN MENU\r\n");
		printf("\nYour choice? ");
		
		while(!usart1_check_RX())
			IWDG_ReloadCounter();
		
		printf("\n\r\n");
		
		switch(usart1_get_byte()) 
		{
			case '1' : Menu_TestSerwo(); break;
            case 's' : ff_test_term(); break;
            case 'x' : return; break;
			default  : Menu_UnknownCommand(); break;
		}
	}
}

//******************************************************************************
//*************************** TEST MODULE MENU *********************************
//******************************** SERWO ***************************************

void Menu_TestSerwo(void)
{
    u16 pulse_lenght = 1000;
    u16 value;

    while (1) 
	{
		IWDG_ReloadCounter();

//		if(TIM_Sys_Flags.TimeDisp200ms)
//		{
//			TIM_Sys_Flags.TimeDisp200ms = 0;

		VT_HIDE_CURSOR;

		printf("\n\r\nTesting Serwo\n\r\n");
		printf(" ------------------------------------ \n\r");
		printf("| 1 | Set impulse lenght[us]|  %6d |\n\r", ADC.milivolt[0]);
		printf("|   | Current           [mA]|  %6d |\n\r", ADC.milivolt[0]);
		printf("|---|-----------------------|-------- \n\r");
		printf("| x | Exit                  |\n\r");
		printf(" --------------------------- \n\r");
		printf("\n\rYour choice? ");

		if(usart1_check_RX())
		{
			printf("\n\r\n");

			VT_SHOW_CURSOR;

			switch (usart1_get_byte())
			{
				case '1' :
					printf("\n\rEnter new value: ");
					value = usart1_get_uint();
					printf("\n\r");

					if(value < 500 || value > 2500)
						break;

					pulse_lenght = value;
					pwm_set_serwo(pulse_lenght);
					break;
				case 'x' :
					return;
					break;
				default  :
					Menu_UnknownCommand();
					break;
			}
		}
		else
		{
			VT_CURSOR_UP(11);
		}
	}    
}

//******************************************************************************
//***************************** ADVANCED MENU **********************************
//******************************************************************************

void Menu_Advanced(void) 
{
	while (1) 
	{	
		printf("\n\r\nADVANCED MENU:\n\r\n");
        printf(" 1 - Communication settings\r\n");
        printf(" 2 - \r\n");
        printf(" 3 - \r\n");
		printf(" x - Back to MAIN MENU\r\n");
		printf("\nYour choice? ");
		
		while(!usart1_check_RX())
			IWDG_ReloadCounter();
		
		printf("\n\r\n");
		
		switch(usart1_get_byte()) 
		{
			case 'x' :
				return;
				break;
			default  :
				Menu_UnknownCommand();
				break;
		}
	}
}

//******************************************************************************
//***************************** ADVANCED MENU **********************************
//************************ COMMUNICATION SETTINGS ******************************

void Menu_Comm_Settings(void)
{

}

//******************************************************************************
//***************************** ADVANCED MENU **********************************
//******************************************************************************


//******************************************************************************
//****************************** RESET MENU ************************************
//******************************************************************************

void Menu_Reset(void) 
{ 
    printf("\n\r RESETING NOW...\n\r");
    while(1);
}

//******************************************************************************
//****************************** EXIT MENU *************************************
//******************************************************************************

void Menu_Exit(void) 
{
    printf("\n\rNORMAL MODE.\n\r");
    printf("Press ENTER to enter menu.\n\n\r");
}

//******************************************************************************
//****************************** UNKNOWN COMMAND MENU **************************
//******************************************************************************

void Menu_UnknownCommand(void) 
{	
    printf("\n\r Unknown command\n\r");
}

//*****************************************************************************
//******************************END OF MENU************************************
//*****************************************************************************



//*****************************************************************************
//***********************Display analog measurements1**************************												   
//*****************************************************************************

void DisplayAnalog1(void)
{
	printf(" ---------------------------- \n\r");
	printf("|AIN           [mV]| %6d |\n\r", ADC.milivolt[0]);
	printf("|AIN           [mV]| %6d |\n\r", ADC.milivolt[1]);
	printf("|AIN           [mV]| %6d |\n\r", ADC.milivolt[2]);
	printf("|AIN           [mV]| %6d |\n\r", ADC.milivolt[3]);
	printf("|AIN           [mV]| %6d |\n\r", ADC.milivolt[4]);
	printf("|AIN           [mV]| %6d |\n\r", ADC.milivolt[5]);
	printf("|AIN           [mV]| %6d |\n\r", ADC.milivolt[6]);
	printf("|AIN           [mV]| %6d |\n\r", ADC.milivolt[7]);
	printf(" ---------------------------- \n\r");		
}
