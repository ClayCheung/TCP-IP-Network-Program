/*
 * =====================================================================================
 *
 *       Filename:  echo_server.c
 *
 *    Description:  × 服务器端在同一时刻只与一个客户端相连，并提供echo服务
 *                  × sever依次向5个client提供服务并退出
 *                  × client接收用户输入的string and send to sever
 *                  × sever rcv any string and 将相同的内容传回client
 *                  × sever and client 之间的echo服务一直执行，直到client用户输入Q为止
 *                  × 版本号为该代码在书的页码（eg:vesion 0.73 === 代码在73页）
 *        Version:  0.73
 *        Created:  2016年10月10日 18时57分01秒
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


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int
main ( int argc, char *argv[] )
{

	int serv_sock,clnt_sock;
	char message[BUF_SIZE];
	int str_len,i;

	struct sockaddr_in serv_adr;
	struct sockaddr_in clnt_adr;
	socklen_t clnt_adr_sz;

	if(argc != 2){
		printf("Usage : %s <port>\n",argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
		error_handling("socket() error");
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
		
	if(listen(serv_sock, 5) == -1)
		error_handling("listen() error");
	
	clnt_adr_sz = sizeof(clnt_adr);
	for(i=0; i<5 ; i++){
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		if(clnt_sock == -1)
		  error_handling("accept() error");
		else
		  printf("Connected client %d \n",i+1);

		while((str_len = read(clnt_sock, message, BUF_SIZE)) != 0)
		  write(clnt_sock, message, str_len);

		close(clnt_sock);
	}
	
	close(serv_sock);

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
	fputc('\n',stderr);
	exit(1);
}		/* -----  end of function error_handling  ----- */













