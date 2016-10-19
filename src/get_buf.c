/*
 * =====================================================================================
 *
 *       Filename:  get_buf.c
 *
 *    Description:  socket option
 *					----SO_SNDBUF & SO_RCVBUF
 *					我目前的理解，就是调整窗口大小的函数
 *					
 *					
 *        Version:  1.43
 *        Created:  2016年10月19日 12时29分17秒
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
	int snd_buf, rcv_buf, state;
	socklen_t len;

	sock = socket(PF_INET, SOCK_STREAM, 0);
	
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





