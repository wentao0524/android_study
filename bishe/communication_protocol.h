#ifndef COMMUNICATION_PROTOCOL_H
#define COMMUNICATION_PROTOCOL_H

#include "socket_base.h"

typedef struct _Uart
{
	void* pDerivedObj;
	
	//interface to access member functions
	p_init_uart_dev uart_init;
	p_send_uart  send_uart;  
	p_rev_uart   rev_uart; 	
	fptrDelete		Delete;
}Uart;

#endif

