/*
 *      AVR447: Sinusoidal driving of three-phase permanent motor using
 *      ATmega48/88/168
 *
 ******************************************************************************/

#ifndef _COMMUTATION_H_
#define _COMMUTATION_H_

//! The number of elements in the sine modulation table per phase.
#define COMMUTATION_TABLE_LENGTH 192U

#if COMMUTATION_TABLE_LENGTH != 192U
#warning "Changing sine modulation table length can have unwanted side effects. Consult the documentation for more information."
#endif



#endif
