#ifndef __TERMINAL_H__
#define __TERMINAL_H__

#include "common.h"

#define TERM_DISPLAY_NORMAL  	0
#define TERM_DISPLAY_ANALOG1   	1
#define TERM_DISPLAY_ANALOG2   	2
#define TERM_DISPLAY_ANALOG3   	3
#define TERM_DISPLAY_DIGITAL   	4
#define TERM_DISPLAY_MENU	 	9

//main display function
void Display(void);

//menu - service mode
void Menu(void);

void Menu_TestModule(void);
    void Menu_TestSerwo(void);
    void Menu_TestZaw2(void);

void Menu_Advanced(void);
    void Menu_Comm_Settings(void); 
    void Menu_CompresorSettings(void);
    void Menu_PIDSettings_CompCurrent(void);

void Menu_Settings(void); 

void Menu_Reset(void); 
void Menu_Exit(void);
void Menu_UnknownCommand(void);

//display analog measurements
void DisplayAnalog1(void);
void DisplayAnalog2(void);
void DisplayAnalog3(void);

void DisplayDigital(void);

// SD card
//void TestModuleSDcard(void);

#endif
