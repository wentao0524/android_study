#ifndef OPERATE_FILE_H
#define OPERATE_FILE_H



typedef struct _Uart Uart;


typedef int	(*p_init_uart_dev)(Uart* const);
typedef int	(*p_send_uart)(Uart* const,char *buf,int len);
typedef int	(*p_rev_uart)(Uart* const,char *buf,int len);
typedef void	(*fptrDelete)(Uart* const) ;

typedef struct _C_uart_base
{
	int uart_speed;
	int uart_wordLength;
	int uart_stopBits;
	int uart_parity;  //∆Ê≈º–£—È
	//interface to access member functions
}C_uart_base;

typedef struct _C_uart_linux
{
	void* pDerivedObj;
	C_uart_base uart_base;
	char puart_dev[32];      
	int fd;
	
	//interface to access member functions
	fptr_init_uart_dev uart_init;
	fptr_init_uart_dev uart_init;
	fptr_send_uart  send_uart;  
	fptr_rev_uart   rev_uart; 	
	fptr_delete   uart_delete;
	
}C_uart_linux;

extern int init_Person(Uart* pObj,const char* pFName, const char* pLName);

#endif