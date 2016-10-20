/*
 * =====================================================================================
 *
 *       Filename:  nodelay_file_server.c
 *
 *    Description:  改造file_server.c
 *					
 *					更改默认socket option ---TCP_NODELAY
 *					禁用Nagle算法，
 *					在大文件传输时，可以提高传输速度
 *
 *        Version:  1.51++
 *        Created:  2016年10月19日 17时33分37秒
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
#include <linux/tcp.h>
void error_handling(char *message);

#define BUF_SIZE 30

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
	int
main ( int argc, char *argv[] )
{
	int serv_sd,clnt_sd;
	FILE *fp;
	char buf[BUF_SIZE];
	int read_cnt;

	int opt_val;
	
	char *clnt_ip;
	int clnt_port;

	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t clnt_adr_sz,
			  opt_len;


	if(argc != 3){
		printf("Usage: %s <port> <File Path> \n",argv[0]);
		exit(1);
	}

	fp = fopen(argv[2], "rb");		//b 代表二进制文件
	if(fp == NULL)
		error_handling("fopen() error");


	serv_sd = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sd == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	if(listen(serv_sd, 5) == -1)
		error_handling("listen() error");

	clnt_adr_sz = sizeof(clnt_adr);
	clnt_sd = accept(serv_sd, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
	if(clnt_sd == -1)
		error_handling("accept() error");
	
	clnt_ip = inet_ntoa(clnt_adr.sin_addr);
	clnt_port = ntohs(clnt_adr.sin_port);	
	printf("client IP: %s \n", clnt_ip);
	printf("client port: %d \n", clnt_port);

	opt_len = sizeof(opt_val);
	if(getsockopt(clnt_sd, IPPROTO_TCP, TCP_NODELAY, (void*)&opt_val, &opt_len))
		error_handling("getsockopt() error!");
	printf("默认TCP_NODELAY 值：%d \n", opt_val);

	opt_val = 1;
	if(setsockopt(clnt_sd, IPPROTO_TCP, TCP_NODELAY, (void*)&opt_val, opt_len))
		error_handling("setsockopt() error!");
	printf("设置 TCP_NODELAY 成功！\n");
	
	if(getsockopt(clnt_sd, IPPROTO_TCP, TCP_NODELAY, (void*)&opt_val, &opt_len))
		error_handling("getsockopt() error!");
	printf("当前 TCP_NODELAY 值：%d \n", opt_val);



	/*-----------------------------------------------------------------------------
	 *   读取fp的BUF_SIZE 字节 到 buf,
	 *   将buf中的30字节内容write到clnt_sd中
	 *   再读取fp中的30字节 到buf,再write 30 字节到 clnt_sd中，
	 *   如此循环，直到最后fp中的内容 小于30 字节，
	 *   比如剩下最后22字节，
	 *   则符合if的条件，write 最后的22字节到clnt_sd里，最后break 跳出循环
	 *-----------------------------------------------------------------------------*/

	while(1){
		read_cnt = fread((void*)buf, 1, BUF_SIZE, fp);
				
		if(read_cnt < BUF_SIZE){
			if(write(clnt_sd, buf, read_cnt) == -1)
				error_handling("write() error");
			break;
		}
		write(clnt_sd, buf, BUF_SIZE);
	}


	/*-----------------------------------------------------------------------------
	 *  下面进行 半关闭 ，关闭输出流，但允许接收数据，
	 *  关闭输出流是为了向clnt_sd 发送 EOF，
	 *  这样 client端 的read()就会读到EOF，就会返回0,
	 *  以返回0为契机，给server发thank you,
	 *  这时 server 因为没有关闭输入流，还可以收到 thank you,
	 *  完美收官，优雅关闭
	 *-----------------------------------------------------------------------------*/

	if(shutdown(clnt_sd, SHUT_WR) == -1)
		error_handling("shutdown() error");
	if(read(clnt_sd, buf, BUF_SIZE) == -1)
		error_handling("read() error");
	printf("Message from client: %s \n",buf);
	
	/*-----------------------------------------------------------------------------
	 *  确定client 收到文件后 ，再关闭fp ,关闭clnt_sd 和 serv_sd
	 *-----------------------------------------------------------------------------*/
	fclose(fp);
	close(clnt_sd);
	close(serv_sd);

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
