/*
 * vt100.h
 *
 *  Created on: 10-09-2013
 *      Author: wacek
 */

#ifndef VT100_H_
#define VT100_H_

//VT100 CSI Sequences---------------------------------------------

#define ESC     0x1B
#define UP      0x1B41
#define DOWN    0x1B42
#define LEFT    0x1B44
#define RIGHT   0x1B43
#define HOME    0x1B7E

#define BACKSPACE 0x08

#define VT_CUP    			printf("\x1b[H");

#define VT_CLEAR_SCREEN    	printf("\x1b[2J")
#define VT_CLEAR_END_LINE  	printf("\x1b[0K")
#define VT_SHOW_CURSOR     	printf("\x1b[?25h")
#define VT_HIDE_CURSOR     	printf("\x1b[?25l")

#define VT_CURSOR_UP(n)    	printf("\x1b["#n"A")
#define VT_CURSOR_DOWN(n)  	printf("\x1b["#n"B")
#define VT_CURSOR_RIGHT(n) 	printf("\x1b["#n"C")
#define VT_CURSOR_LEFT(n)  	printf("\x1b["#n"D")
#define VT_CURSOR_00       	printf("\x1b[?6h")

#define VT_CHAR_NORMAL     	printf("\x1b[0m")

#define VT_SET_BOLD        	printf("\x1b[1m")
#define VT_SER_UNDERLINE   	printf("\x1b[4m")
#define VT_SET_BLINK       	printf("\x1b[5m")
#define VT_SET_INVERSE     	printf("\x1b[7m")

#define VT_RES_BOLD        	printf("\x1b[22m")
#define VT_RES_UNDERLINE   	printf("\x1b[24m")
#define VT_RES_BLINK       	printf("\x1b[25m")
#define VT_RES_INVERSE     	printf("\x1b[27m")

#define BLACK   0
#define RED     1
#define GREEN   2
#define YELLOW  3
#define PURPLE  4
#define PINK    5
#define BLUE    6
#define WHITE   7
#define DEF     9

#define COLOR(n) #n
#define VT_CHAR_COLOR(n)   	printf("\x1b[3"COLOR(n)"m")
#define VT_BACK_COLOR(n)   	printf("\x1b[4"COLOR(n)"m")

#define VT100_setcursorzero() 	VT_CURSOR_00
#define VT100_setcolor(n) 		printf("\x1b[4"COLOR(n)"m")


#endif /* VT100_H_ */
