/* Martin Thomas 4/2009, 3/2010 */
#include "comm.h"

#define USARTx USART1

int comm_test(void)
{
	return ( USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET ) ? 0 : 1;
}

char comm_get(void)
{
    //printf("comm_get 0\n\r");
	while(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET) 
    { 
    
    }
    //printf("comm_get 1\n\r");
	return (char)USART_ReceiveData(USARTx);
}

void comm_put(char d)
{
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET) { ; }
	USART_SendData(USARTx, (u16)d);
}

void comm_puts(const char* s)
{
	char c;
	while ( ( c = *s++) != '\0' ) {
		comm_put(c);
	}
}

void comm_init (void)
{
	// already done in main.c
}


