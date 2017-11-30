// server.cpp : Defines the entry point for the console application.
//

#include "socket_base.h"
#include "uart.h"


#define SHORT_STR 128

struct s_sock_args sock_args;

int get_and_parse_args(int cmd_value)
{

	int ret=0;
	//char * ptr_args=NULL;
	//ptr_args=&(sock_args.args_buf[0]);
	switch(cmd_value)
	{

	case 1000://test 
		{
			puts(sock_args.args_buf);
			memset(sock_args.args_buf,0,sizeof(sock_args.args_buf));
			break;
		}
	case 2000://image 
		{
			break;
		}
	case 3000://sound
		{
			break;
		}
	case 6000://close  
		{
			break;
		}
	default://error
		{
			ret=-1;
			break;
		}
	}
	return ret;
}
int check_cmdvalue_and_key(int cmd_value,int key)
{
	int ret=-1;
	ret=0;
	return ret;
}
int wait_query()
{
	int ret=-1;
	int read_byte=0;
	int pkg_len=-1;
	int temp_cmdvalue=0;
	int temp_key=0;
	SOCK_TYPE sock =sock_args.sock;
    char buf[PKG_HEAD_BUF_SIZE];
	memset(buf,0,sizeof(buf));
	read_byte = readn(sock,buf,PKG_HEAD_BUF_SIZE);
	if(PKG_HEAD_BUF_SIZE==read_byte)
	{
		memcpy(sock_args.pkg_head,buf,PKG_HEAD_BUF_SIZE);
		memcpy(&pkg_len,sock_args.pkg_head+8,4);
//		ret=check_cmdvalue_and_key(temp_cmdvalue,temp_key);
        if(read_byte >0)
		{
			int left_byte=pkg_len-PKG_HEAD_BUF_SIZE;
			if(0==left_byte)
			{
				ret=pkg_len;
			}
			else
			{
				if((left_byte>0)&&(left_byte < (ARGS_BUF_SIZE-8)))
				{
					memset(sock_args.args_buf,0,sizeof(sock_args.args_buf));
				//	printf("%d\n",sizeof(sock_args.args_buf));
					read_byte=readn(sock,sock_args.args_buf,left_byte);
					if(read_byte==left_byte)
					{
						ret=pkg_len;
					}
					else
					{
						ret=read_byte;
					}
				}
				else
				{
					ret=-1;
				}
			}
		}
	}
	else
	{
		ret=read_byte;
		printf("read byte is %d\n",read_byte);
	}	
	if(ret>0)
	{
		sock_args.cmd_value=temp_cmdvalue;
		sock_args.key=temp_key;
		sock_args.pkg_len=pkg_len;
	}
	return ret;
}
SOCK_TYPE client_sock_init(char * str_ip,int port)
{
	SOCK_TYPE sClntSock=init_connect(str_ip,port);
//	int ret=set_socket_timewait_one_minute(sClntSock);
    int ret=0;
	if(0!=ret)
	{
		sClntSock=-1;
	}
	return sClntSock;
}

int wait_response()//recv data
{
	int ret=-1;
	int read_byte=0;
	int pkg_len=-1;
	int temp_cmdvalue=0;
	int temp_key=0;
	SOCK_TYPE sock =sock_args.sock;
    char buf[MESSAGE_HEADER];
	memset(buf,0,sizeof(buf));
	read_byte = readn(sock,buf,MESSAGE_HEADER);
	if(MESSAGE_HEADER==read_byte)
	{
		memcpy(&temp_cmdvalue,buf,4);
		memcpy(&temp_key,buf+4,4);
		memcpy(&pkg_len,buf+8,4);
		ret=check_cmdvalue_and_key(temp_cmdvalue,temp_key);
        if(0==ret)
		{
			int left_byte=pkg_len-MESSAGE_HEADER;
			if(0==left_byte)
			{
				ret=pkg_len;
			}
			else
			{
				if((left_byte>0)&&(left_byte < (ARGS_BUF_SIZE-8)))
				{
					read_byte=readn(sock,sock_args.args_buf,left_byte);
					if(read_byte==left_byte)
					{
						ret=pkg_len;
					}
					else
					{
						ret=read_byte;
					}
				}
				else
				{
					ret=-1;
				}
			}
		}
	}
	else
	{
		ret=read_byte;
		printf("read byte is %d\n",read_byte);
		if(RECV_ERROR==ret)
		{
			sock_args.sock=-1;
		}
	}	
/*	if(ret>0)
	{
		sock_args.cmd_value=temp_cmdvalue;
		sock_args.key=temp_key;
		sock_args.pkg_len=pkg_len;
	}
	*/
	return ret;
}

int send_query()
{
	int ret=-1;	
	int buf_len=PKG_HEAD_BUF_SIZE;
	char buf[PKG_HEAD_BUF_SIZE];
	int args_size=sock_args.pkg_len-PKG_HEAD_BUF_SIZE;
	
/*	memset(buf,0,sizeof(buf));
	memcpy(buf,&(sock_args.cmd_value),4);
	memcpy(buf+4,&(sock_args.key),4);
	memcpy(buf+8,&(sock_args.pkg_len),4);
	*/
	ret=send_data(sock_args.sock,sock_args.pkg_head,PKG_HEAD_BUF_SIZE);
	
	if(0==ret)
	{
		if((args_size>0)&&(args_size<ARGS_BUF_SIZE-PKG_HEAD_BUF_SIZE))
		{
			ret=send_data(sock_args.sock,sock_args.args_buf,args_size);
		}
	}
	if(SEND_ERROR==ret)
	{
		sock_args.sock=-1;
	}
	return ret;	 

}
void mygets(char *str)
 {
	 int i=0;
	 scanf("%c",&str[i]);
	 while(str[i++]!='\n')
	 {
		scanf("%c",&str[i]);
	 }
	 str[i-1]=0;//  \0是字符数组中默认的字符串结尾，\0所对应的ascii码为0
 }



int get_input_data_and_send()
{   
	int ret=-1;
	int len=0;
	if(0!=sock_args.sock)
	{
		char * p_buf=sock_args.args_buf;
		memset(&sock_args.args_buf,0,ARGS_BUF_SIZE);
		printf("please input sth!\n");
		gets(p_buf);
	//	mygets(p_buf);
		len=strlen(p_buf);
		sock_args.cmd_value=1000;
		sock_args.pkg_len=len+MESSAGE_HEADER;
		ret=send_query();
		
	}
	return ret;
}
int get_net_data_and_printf()
{
	int ret=-1;
	if(0!=sock_args.sock)
	{
		ret=wait_response();
		puts(sock_args.args_buf);
	}
	return ret;
}

int client_business(int uart_fd)
{
	int ret=-1;
	int pkg_len=sock_args.pkg_len;
	int args_len=0;
	char buf[4];
	memset(buf,0,sizeof(buf));
	args_len=pkg_len-PKG_HEAD_BUF_SIZE;
	if(args_len>0)
	{
		uart_writen(uart_fd,sock_args.args_buf,args_len);
		memset(sock_args.args_buf,0,sizeof(sock_args.args_buf));
		uart_readn(uart_fd,buf,sizeof(buf));
		memcpy(&sock_args.pkg_len,buf,sizeof(buf));
		if(sock_args.pkg_len>0)
		{
			uart_readn(uart_fd,sock_args.args_buf,sock_args.pkg_len);
			sock_args.pkg_len+=PKG_HEAD_BUF_SIZE;
			memcpy(sock_args.pkg_head+8,&sock_args.pkg_len,4);
		}
	}
	return ret;
}

int sock_business_client_loop(void)
{
	int ret=-1;

	//const char * str_ip="127.0.0.1";
	const char * str_ip="192.168.8.207";
    int cmd_port=9000;
	int uart_fd=0;
	sock_args.sock=-1;
	sock_args.sock=client_sock_init((char *)str_ip,cmd_port); 
	uart_fd=my_uart_init(9600);
	while(-1!=sock_args.sock)
	{
		if(uart_fd>0)
		{
			ret=wait_query();           //等待服务器发送数据
			if(ret > 0)
			{
				ret=client_business(uart_fd);
				send_query();
			}  
		}
	}  
	return ret;
}
int main(int argc, char* argv[])
{
	int fp_t=0;
	char order[]="mjpg_streamer -i \"input_uvc.so -f 30 -r 1280*720\" -o \"output_http.so -p 8080\" ";
	fp_t=popen(order,"r");
	if (!fp_t)
	{
  //      return -1;
    }
	sock_business_client_loop();
	printf("Hello World!\n");
	return 0;
}
