/*
 * =====================================================================================
 *
 *       Filename:  echo_client.c
 *
 *    Description:  × 服务器端在同一时刻只与一个客户端相连，并提供echo服务
 *                  × sever依次向5个client提供服务并退出
 *                  × client接收用户输入的string and send to sever
 *                  × sever rcv any string and 将相同的内容传回client
 *                  × sever and client 之间的echo服务一直执行，直到client用户输入Q为止
 *                  × 版本号为该代码在书的页码（eg:vesion 0.73 === 代码在73页）
 *        Version:  0.83
 *        Created:  2016年10月10日 22时18分15秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  ClayZhang (伦), 
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


#include	<stdlib.h>

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
	char message[BUF_SIZE];
	int str_len,recv_len,recv_cnt;
	struct sockaddr_in serv_adr;

	if(argc != 3 ){
		printf("Usage : %s <IP> <port>\n",argv[0]);
		exit(1);
	}	
	
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1)
	  error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
	  error_handling("connect() error!");
	else
	  puts("connected...........");

	while(1){
		fputs("Input message(Q to quit):",stdout);
		fgets(message, BUF_SIZE, stdin);

		if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))
		  break;

    	str_len = write(sock, message, strlen(message));
		
		recv_len = 0;
		while(recv_len < str_len){
		    recv_cnt = read(sock, message, BUF_SIZE-1);
		    if(recv_cnt == -1)
			  error_handling("read() error!");
			recv_len += recv_cnt;
		}
			message[recv_len] = '\0';
		    printf("Message from server: %s",message);
	}
	
	close(sock);
	
	return 0 ;

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
	fputc('\n',stderr);
	exit(1);
}		/* -----  end of function error_handling  ----- */









