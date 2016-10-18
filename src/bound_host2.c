/*
 * =====================================================================================
 *
 *       Filename:  bound_host2.c
 *
 *    Description:  udp_host2
 *					send message to host1
 *					× udp 有数据边界， 一边sendto 3次
 *									, 另一边必须 recvfrom 3次
 *					
 *					
 *					$ 未连接的UDP套接字：sendto(sock, buff, buff_size, flags
 *												address, addrlen)
 *						sendto()做了三件事：
 *								1. 向UDP套接字 注册 目标IP port
 *								2. 传输数据
 *								3. 删除 注册信息
 *						第一阶段和第三阶段，占了整个通信过程的1/3的时间，
 *						如果需要向同一个目标多次传输数据，
 *						则会在1和3 两个阶段浪费太多时间（并不需要多次注册多次删除）
 *					
 *					× 这种情况下 ，可以一次注册 多次传输数据 再最后删除注册
 *					  也就是使用
 *					  
 *					$ 已连接的套接字：
 *								1.注册 : connect()
 *								2.传输 ：write() read()
 *								3.删除注册，删除套接字 ：close()
 *					
 *					× 小结：传输对象目标固定，且需要多次传输接收数据的情况下，
 *							使用 已连接的UDP套接字 比 未连接的UDP套接字 ，
 *							将会大大提高性能
 *								
 *							
 *			
 *        Version:  1.12
 *        Created:  2016年10月18日 11时08分51秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Clay Zhang (伦), 
 *   Organization:  
 *
 * =====================================================================================
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *err_message);



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

	char msg1[] = "Hi!";
	char msg2[] = "I'm another UDP host!";
	char msg3[] = "Nice to meet you";

	struct sockaddr_in your_adr;
	//socklen_t your_adr_sz;
	
	if(argc != 3){
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(sock == -1)
		error_handling("socket() error");
	
	memset(&your_adr, 0, sizeof(your_adr));
	your_adr.sin_family = AF_INET;
	your_adr.sin_addr.s_addr = inet_addr(argv[1]);
	your_adr.sin_port = htons(atoi(argv[2]));
	
	connect(sock, (struct sockaddr*)&your_adr, sizeof(your_adr));

	write(sock, msg1, strlen(msg1));
	/*
		sendto(sock, msg1, sizeof(msg1), 0,
			(struct sockaddr*)&your_adr, sizeof(your_adr));
	*/

	write(sock, msg2, strlen(msg2));
	/*
		sendto(sock, msg2, sizeof(msg2), 0,
			(struct sockaddr*)&your_adr, sizeof(your_adr));
	*/
	
	write(sock, msg3, strlen(msg3));
	/*
		sendto(sock, msg3, sizeof(msg3), 0,
			(struct sockaddr*)&your_adr, sizeof(your_adr));
	*/
	close(sock);

	return 0;
}				/* ----------  end of function main  ---------- */



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  error_handling
 *  Description:  
 * =====================================================================================
 */
	void
error_handling (char *err_message)
{
	fputs(err_message, stderr);
	fputc('\n', stderr);
	exit(1);
}		/* -----  end of function error_handling  ----- */
