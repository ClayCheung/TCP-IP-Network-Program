/*
 * =====================================================================================
 *
 *       Filename:  set_buf.c
 *
 *    Description:  socket option
 *					设置 SO_SNDBUF & SO_RCVBUF
 *					by setsockopt() 函数
 *					
 *					set 的参数会 乘以2 设置为实际的buffer大小
 *					输入缓冲和输出缓冲 都有各自的最大值和最小值
 *					可以通过这个函数，测试出最大值最小值
 *					
 *        Version:  1.44+
 *        Created:  2016年10月19日 12时48分45秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Clay Zhang (伦), 
 *   Organization:  
 *
 * =====================================================================================
 */


#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<sys/socket.h>

void error_handling(char *message);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
	int
main ( int argc, char *argv[] )
{	
	int sock;
	int state;
	socklen_t len;

	
	if(argc != 3){
		printf("Usage: %s <value of rcv_buf> <value of snd_buf> \n", argv[0]);
		exit(1);
	}

	int rcv_buf = atoi(argv[1]);
	int snd_buf = atoi(argv[2]);	
	
	sock = socket(PF_INET, SOCK_STREAM, 0);

	state = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, sizeof(snd_buf));
	if(state)
		error_handling("setsockopt() error!");
	
	state = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, sizeof(rcv_buf));
	if(state)
		error_handling("setsockopt() error!");


	len = sizeof(snd_buf);
	state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, &len);
	if(state)
		error_handling("getsockopt() error");

	len = sizeof(rcv_buf);
	state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, &len);
	if(state)
		error_handling("getsockopt() error");

	printf("Input buffer size: %d \n", rcv_buf);
	printf("Output buffer size: %d \n", snd_buf);

	return 0;
}				/* ----------  end of function main  ---------- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  error_handling
 *  Description:  
 * =====================================================================================
 */
	void
error_handling (char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}		/* -----  end of function error_handling  ----- */





