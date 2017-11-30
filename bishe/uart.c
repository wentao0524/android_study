#include <stdio.h>
#include <stdlib.h>
#include <termio.h>
#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include "uart.h"


//#define BOARD_2440

#define FALSE -1
#define TRUE  1
//int speed_arr[] = { B38400, B19200, B9600, B4800, B2400, B1200, B300, //
//B38400, B19200, B9600, B4800, B2400, B1200, B300, };
//int name_arr[] = {38400, 19200, 9600, 4800, 2400, 1200, 300,
//38400, 19200, 9600, 4800, 2400, 1200, 300, };
int speed_arr[] = { B115200, B57600, B38400, B19200, B9600, B4800, B2400, B1200, B300,};
int name_arr[] = {115200, 57600, 38400,  19200,  9600,  4800,  2400,  1200,  300,};
void set_speed(int fd, int speed)
{
 int i;
 int status;
 struct termios Opt;
 tcgetattr(fd, &Opt);
 for ( i= 0; i < sizeof(speed_arr) / sizeof(int); i++)
 {
  if (speed == name_arr[i])
  {
   tcflush(fd, TCIOFLUSH);
   cfsetispeed(&Opt, speed_arr[i]); 
   cfsetospeed(&Opt, speed_arr[i]);
   status = tcsetattr(fd, TCSANOW, &Opt); 
   if (status != 0)
    perror("tcsetattr fd1");
   return;
  }
  tcflush(fd,TCIOFLUSH); //¨ª?¨¦?
 }
}

int set_Parity(int fd,int databits,int stopbits,int parity)
{
 struct termios options; //?¡§¨°?¨°????¨¢11
 

 if ( tcgetattr( fd,&options) != 0) //¨º¡Á?¨¨?¨¢¨¨??¦Ì¨ª3??¨¨?¨¦¨¨??options?D,¡À?D?
 {
  perror("SetupSerial 1");
  return(FALSE);
 }
 
 
 options.c_iflag&=~(IXON|IXOFF|IXANY);
 options.c_lflag&=~(ICANON|ECHO|ECHOE|ISIG);
 options.c_iflag&=~(INLCR|ICRNL|IGNCR);
 options.c_oflag&=~(ONLCR|OCRNL);
 
 options.c_cflag &= ~CSIZE; //?a¨º?¨¦¨¨??c_cflag????2?¡ã¡ä??¨ºy?Y???¨²??
 switch (databits)
 {
 case 7:
  options.c_cflag |= CS7; //¨¦¨¨??c_cflag????¨ºy?Y???a7??
  break;
 case 8:
  options.c_cflag |= CS8; //¨¦¨¨??c_cflag????¨ºy?Y???a8??
  break;
 default:
  fprintf(stderr,"Unsupported data size\n"); //????¦Ì???2??¡ì3?
  return (FALSE);
 }
 switch (parity) //¨¦¨¨??????D¡ê?¨¦¡ê?c_cflago¨ªc_iflag¨®DD¡ì
 {
 case 'n':
 case 'N':
  options.c_cflag &= ~PARENB;
  options.c_iflag &= ~INPCK;
  break;
 case 'o':
 case 'O': options.c_cflag |= (PARODD | PARENB);
  options.c_iflag |= INPCK;
  break;
 case 'e':
 case 'E':
  options.c_cflag |= PARENB;
  options.c_cflag &= ~PARODD;
  options.c_iflag |= INPCK;
  break;
 default:
  fprintf(stderr,"Unsupported parity\n");
  return (FALSE);
 }

switch (stopbits)
 {
 case 1:
  options.c_cflag &= ~CSTOPB;
  break;
 case 2:
  options.c_cflag |= CSTOPB;
  break;
 default:
  fprintf(stderr,"Unsupported stop bits\n");
  return (FALSE);
 }

if (parity != 'n')
  options.c_iflag |= INPCK;


 options.c_cc[VTIME] = 150; // 15 seconds
 options.c_cc[VMIN] = 0;

tcflush(fd,TCIFLUSH);
 if (tcsetattr(fd,TCSANOW,&options) != 0)
 {
  perror("SetupSerial 3");
  return (FALSE);
 }
 return (TRUE);
}

int OpenDev(char *Dev)
{
    int fd = open( Dev, O_RDWR ); //| O_NOCTTY | O_NDELAY?a??¡¤?¨º??¡äopeno¡¥¨ºy
    if (-1 == fd)
    {
  perror("Can't Open Serial Port");
  return -1;
    }
    else
  return fd;
}
int my_uart_init(int bit)
{
	int fd;
	#ifdef BOARD_2440
    char *dev ="/dev/ttySAC1";
	#else
	char *dev ="/dev/ttyS0";
	#endif
    fd = OpenDev(dev);
    if (fd>0)
    {
	 printf("Open Serial succeed\n");
	 set_speed(fd,bit);
    }
    else
    {
	  printf("111Can't Open Serial Port!\n");
	  exit(0);
    }
    if (set_Parity(fd,8,1,'N')== FALSE)
    {
	  printf("Set Parity Error\n");
	  exit(1);
    }
	return fd;
}

void uart_send(int fd)
{
	int nread;
    char buff[512];
	while(1)
	{
		memset(buff,0,sizeof(buff));
		scanf("%s",buff);
		nread=strlen(buff);
//		buff[nread]='\n';
		printf("|--%d %s\n",nread,buff);
		write(fd,buff,nread);
	}
}

void uart_recv_send(int fd)
{
	int nread;
    char buff[512];
	while(1)
    {
		 int i;
		 int flag=1;
		 memset(buff,0,sizeof(buff));
		 while((nread = read(fd,buff,10))>0)
		 {
		//	 if((1==nread)&&(0x0a==buff[0])&&(flag==1))
			 {
		//		flag=0;
		//		printf("|-----> 0x%x\n",buff[0]);
			 }
		//	 else
			 {
	//				printf("\nread Len %d\n",nread);
			  
					for(i=0;i<nread;i++)
					{
						printf("%c",buff[i]);
					}
//					printf("\n");
					i = write(fd,buff,nread);		
	//				printf("\nwrite Len %d\n",i);
			 }
		 }
    }
}


void uart_recv_send_big_data(int fd)
{
	int nread=0;
        char buff[512+16];

	int i;
	int total_count=0;
		
	while(1)
	 {
 		memset(buff,0,sizeof(buff));
		while((nread = read(fd,buff+total_count,200))>0)
		{
			printf("|nread--->%d\n",nread);
			total_count+=nread;
			if(total_count>=512)
			{
				break;
			}
		}
		printf("|total_count--->%d\n",total_count);
		for(i=0;i<total_count;i++)
		{
			printf("%c",buff[i]);
		}	
		i = write(fd,buff,total_count);	
		total_count=0;
	 }
}
void uart_send_recv(int fd)
{
	int nread;
    char buff[512];
	int i=0;
	int flag=0;
	while(1)
	{
		memset(buff,0,sizeof(buff));
		scanf("%s",buff);
		nread=strlen(buff);
		buff[nread]='\n';
		write(fd,buff,nread+1);
		flag=nread+1;
		nread=0;
		memset(buff,0,sizeof(buff));
		while((nread = read(fd,buff,100))>0)
		{
//			if((1==nread)&&(0x0a==buff[0])&&(flag==1))
//			 {
//				flag=0;
//				printf("|-----> 0x%x\n",buff[0]);
//			 }
//			 else
//			 {
				printf("read %d\n",nread);
				for(i=0;i<nread;i++)
				{
				  printf("%c",buff[i]);
				}
//				if(flag-nread==0)
				{
					break;
				}
//		     }
/*			memset(buff,0,sizeof(buff));
			scanf("%s",buff);
			nread=strlen(buff);
			buff[nread]='\n';
			write(fd,buff,nread+1);
			nread=0;
			i=0;
*/
		}
	}
}

int uart_recv(int fd)
{
	int nread;
    char buff[512];
	while(1)
    {
		 int i;
		 memset(buff,0,sizeof(buff));
		 while((nread = read(fd,buff,10))>0)
		 {
		//	 if((1==nread)&&(0x0a==buff[0]))
			 {
				//printf("\n|-----> %x\n\n",buff[0]);
			 }
		//	 else
			 {
					printf("\nread Len %d\n",nread);
					for(i=0;i<nread;i++)
					{
						printf("%c",buff[i]);
					}
					printf("\n");
					printf("start!======>\n");
			 }
		 }
    }

}

int uart_recv1(int fd)
{
	
	return 0;
}
/*
int main(int argc, char **argv)
{
	int fd;
	printf("uart test1\n");
	fd=my_uart_init();
//	uart_recv(fd);
//	uart_send(fd);
//	uart_recv_send(fd);
//	uart_send_recv(fd);
	uart_recv_send_big_data(fd);
	close(fd);
}
*/

int uart_readn(int fd,char *buf,int data_len)
{
	int total_count=0;
	int nread=0;
	while((nread = read(fd,buf+total_count,data_len))>0)
	{
		total_count+=nread;
		if(total_count>=data_len)
		{
			break;
		}
	}
	return total_count;	
}

int uart_writen(int fd,char *buf,int data_len)
{
	int ret=-1;
	if(buf!=NULL)
	{
		ret=write(fd,buf,data_len);
	}
	return ret;
}



