#ifndef SOCKET_H
#define SOCKET_H
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/timeb.h>
#include <time.h>

#define DLL_EXPORT

#define ERROR  -1
#define SOCK_TYPE int        //int


#define ARGS_BUF_SIZE 64*64*3+16
#define MESSAGE_HEADER  12
#define SEND_ERROR -1010
#define RECV_ERROR -1011

#define PASSWD_BUF_SIZE   24
#define PKG_HEAD_BUF_SIZE   40


struct s_sock_args
{
	SOCK_TYPE sock;
	int cmd_value;
	int key;
	int pkg_len;
	int plaintext_len;
	char passwd_buf[PASSWD_BUF_SIZE];
	char pkg_head[PKG_HEAD_BUF_SIZE];
	char args_buf[ARGS_BUF_SIZE];
};

DLL_EXPORT int send_data(SOCK_TYPE snew,char *buf,int send_byte);
//*****************************************

DLL_EXPORT int writen(SOCK_TYPE fd, const char *vptr, int n);
DLL_EXPORT int readn(SOCK_TYPE fd,  char *vptr, int total_read_len);



DLL_EXPORT void my_sleep(int sec);
DLL_EXPORT int close_connect(SOCK_TYPE sock);

//server
DLL_EXPORT SOCK_TYPE init_listen(const char * strport);
DLL_EXPORT SOCK_TYPE wait_for_connect(SOCK_TYPE sock);
//client

DLL_EXPORT SOCK_TYPE init_connect(char * str_ip,int port);


/*
extern int writen(SOCK_TYPE fd, const char *vptr, int n);
extern int readn(SOCK_TYPE fd,  char *vptr, int total_read_len);

//___________________________________

extern void my_sleep(int sec);
extern int set_socket_timewait_one_minute(SOCK_TYPE sock);
extern int close_connect(SOCK_TYPE sock);

//server
extern SOCK_TYPE init_listen(const char * strport);
extern SOCK_TYPE wait_for_connect(SOCK_TYPE sock);
//client

extern SOCK_TYPE init_connect(char * str_ip,int port);

*/

#endif



