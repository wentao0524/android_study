#include "socket_base.h"

int writen(int fd, const char *vptr, int n)
{
	size_t nleft;
	ssize_t nwritten;
	const char *ptr;
	
	ptr = vptr;
	nleft = n;
	while (nleft > 0)
	{
		if ((nwritten = write(fd, ptr, nleft)) <= 0)
		{
			if (errno == EINTR || errno == EAGAIN)
			{
				nwritten = 0;	 /* and call writen again */
			}
			else
			{
				printf("write error:%s\n", strerror(errno));
				return  - 1;	 /* error*/
			}
		}
		nleft -= nwritten;
		ptr += nwritten;
	}
	return n;
}

//char read_buf[500*1024];
//char read_buf[TCP_MSS + 1]; 
int readn(int fd,  char *vptr, int total_read_len)
{
	//    char read_buf[300];
//	memset(read_buf,0,sizeof(read_buf));
	int left_len=0;
	int read_cnt = 0;
	
//	char * ptr_buf=read_buf;
	char * ptr_buf=vptr;
	left_len=total_read_len;
	while(left_len>0)
	{
read_again:
        read_cnt = (int)read(fd,ptr_buf,1);
        if(read_cnt<=0)
        {
		// if(errno==EINTR)
		if (errno == EINTR || errno == EAGAIN)
		{
			goto read_again;
		}
		printf("read error:%s\n", strerror(errno));
		return -1;
        }
        left_len-=read_cnt; 
        ptr_buf++;        
	}
//	memcpy(vptr, read_buf, total_read_len);
	return total_read_len;
	
}

int wait_for_connect(int sock)
{
    struct sockaddr_in clientAddr; /* client's address */
    socklen_t client_len; 
    int snew=0; /* socket fd's */
    char addrbuf[17];
    bzero(&clientAddr, sizeof(clientAddr));
    
    client_len = sizeof(clientAddr);
    snew = accept(sock, (struct sockaddr*) &clientAddr, &client_len);
    printf (" --------------------\n");
    printf ("Client socket %d connected...\n", snew);
#if 0
    if (snew < 0 && errno == EINTR)
        continue;
    else if (snew < 0)
    {
        printf ("accept failed\n");
        break;
    }
#else
    if (snew < 0)
    {
        printf ("accept failed\n");
        return -1;
    }
#endif
    
    inet_ntop(AF_INET,&clientAddr.sin_addr,addrbuf,sizeof(addrbuf));
    
    printf ("Client Info: port = %d: family = %d: addr = %s:\n", 
           ntohs(clientAddr.sin_port), clientAddr.sin_family, addrbuf);
    printf ("|---wait_for_connect %d !\n",snew);
    return snew;
}

int init_listen(const char * strport)
{
    struct sockaddr_in serverAddr; /* server's address */
    unsigned short portnum=0;
    int b_reuse = 1;
    int sock=-1;
    /* Set up our internet address, and bind it so the client can connect. */
	portnum = atoi(strport);
    
    /* Zero out the sock_addr structures.
	 * This MUST be done before the socket calls. */
	bzero(&serverAddr, sizeof(serverAddr));
	/* Open the socket.  Use ARPA Internet address format and stream sockets.   */
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == ERROR)
	{
		printf ("socket failed, error:%s\n", strerror(errno));
        return -1;
	}
  
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &b_reuse, sizeof(int)) < 0)
	{
		printf ("error %s setting SO_REUSEADDR on port %d\n", strerror(errno), portnum);
        return -1;
	}

	//portnum = atoi(argv[1]);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(portnum);
	printf("\nBinding server port %d\n", portnum);
    
	if (bind(sock, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) != 0)
	{
		printf ("bind failed, error:%s\n", strerror(errno));
		close(sock);
        return -1;
	}
    
	/* Listen, for the client to connect to us. */
	printf("Listening for connect...\n");
	if (listen(sock, 2) != 0)
	{
		printf ("listen failed\n");
		close(sock);
        return -1;
	}    
    return sock;
}
/*
void get_hostent(char * hostName,struct hostent **ret_hptr)
{
	//struct hostent ** ret_hptr; // ptr to addr struct 
	struct hostent *hptr; // ptr to addr struct 
	if ((hptr = gethostbyname(hostName)) == NULL)
	{
		fprintf(stderr, "gethostbyname error for host %s!\n", hostName);
		exit(1);
	}
	*ret_hptr=hptr;
}
*/
int init_connect(char * str_ip,int port)
{
	int sock; /* socket fd's */
	struct sockaddr_in serv_addr; /* server's address */
	char addr_buf[17];
	//struct hostent *hptr=NULL; // ptr to addr struct 
	//get_hostent(str_ip,&hptr);
	/* Zero out the sock_addr structure */
	bzero(&serv_addr, sizeof(serv_addr));
	
	/* Open the socket */
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == ERROR)
	{
		fprintf(stderr, "socket failed, errno = %d\n", errno);
		exit(1);
	}
	/* Set up the server's internet address */
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	//		serv_addr.sin_addr = *(struct in_addr*) * hptr->h_addr_list;
//	serv_addr.sin_addr = *(struct in_addr *)hptr->h_addr;
	serv_addr.sin_addr.s_addr =inet_addr(str_ip) ;
	inet_ntop(AF_INET,&serv_addr.sin_addr, addr_buf,sizeof(addr_buf));
	
	printf("Server's address is %s\n", addr_buf);
	
	if (connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) != 0)
	{
		printf("Connect failed:\n");
		shutdown(sock, 2);
		close(sock);
		exit(1);
	}
	// printf("Connected...\n  Press <ESC> to exit \n\n");
	printf("Connected ok!\n");
	return sock;
}

int GetTickCount(void)
{
	int time_count=0;
	struct timeb time_elp;
	ftime(&time_elp);
//	time_count = int(time_elp.time * 1000 + time_elp.millitm);
	return time_count;
}

int set_sock_time_wait(SOCK_TYPE sock)
{
	int ret=-1;
    struct timeval tv; 
    tv.tv_sec = 20;
    tv.tv_usec = 0;
    ret=setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    return ret;
}

void my_sleep(int sec)
{

	sleep(sec);

}
int close_connect(SOCK_TYPE sock)
{
	/* close the socket */
	printf("close the socket!\n");

	shutdown(sock, 2);
	close(sock);

	return 0;
}

int send_data(SOCK_TYPE snew,char *buf,int send_byte)
{
    int ret=-1;
    int write_byte=0;	
	/* send back the data to the socket*/
	write_byte = writen(snew, buf,send_byte);
	if (write_byte == send_byte)
	{
//        DLog (@"Send back the result %i \n",write_byte);
        ret=0;
	}
	else
	{
		/* client has disappeared */
		if ((errno != EINTR) && (errno != EAGAIN))
		{
            printf("error:%s\n", strerror(errno));
		}
		close_connect(snew);
        printf("|--- error Send back the result %i \n",write_byte);
		ret=write_byte;
	}
	return ret;	 
}


