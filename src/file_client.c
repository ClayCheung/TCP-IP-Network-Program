/*
 * =====================================================================================
 *
 *       Filename:  file_client.c
 *
 *    Description:  half-close
 *    
 *					client端
 *
 *        Version:  1.23
 *        Created:  2016年10月18日 22时05分56秒
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
#include	<string.h>
#include	<unistd.h>
#include	<arpa/inet.h>
#include	<sys/socket.h>

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
	int sd;
	FILE *fp;

	char buf[BUF_SIZE];
	int read_cnt;
	struct sockaddr_in serv_adr;

	if(argc != 3){
		printf("Usage: %s <IP> <port> \n", argv[0]);
		exit(1);
	}

	fp = fopen("/home/clay/receive.dat", "wb");		//fopen 一个新的空的可写入的二进制文件
	sd = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	connect(sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr));

	/*-----------------------------------------------------------------------------
	 *  没有到文末（没收到EOF），就一直循环read()然后fwrite()
	 *-----------------------------------------------------------------------------*/
	while((read_cnt = read(sd, buf, BUF_SIZE)) != 0)
		fwrite((void*)buf, 1, read_cnt, fp);

	puts("Received file data in ~/receive.dat");
	write(sd, "Thank you", 10);

	fclose(fp);
	close(sd);
	
	return 0;
}				/* ----------  end of function main  ---------- */






