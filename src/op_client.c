/*
 * =====================================================================================
 *
 *       Filename:  op_client.c
 *
 *    Description:  
 *
 *        Version:  0.87
 *        Created:  2016年10月12日 09时46分36秒
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
#define RLT_SIZE 4
#define OPSZ 4
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
	char opmsg[BUF_SIZE];
	int result;
	int opnd_cnt;
	int i;
	struct sockaddr_in serv_adr;
	if(argc != 3){
		printf("Usage : %s <IP> <port>\n",argv[0]);
		exit(1);
	}
	
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1)
	  error_handling("socket() error");

	memset(&serv_adr, 0 ,sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
	  error_handling("connect() error!");
	else
	  puts("connected.........");

	fputs("Operand count: ",stdout);
	scanf("%d", &opnd_cnt);
	opmsg[0] = (char)opnd_cnt;

	for(i=0; i<opnd_cnt; i++){
		printf("Operand %d: ",i+1);
		scanf("%d", (int*)&opmsg[i*OPSZ+1]);
	}
	fgetc(stdin);//后面需要再向opmsg中输入字符，在此之前调用fgetc函数删掉缓冲中的字符\n
	fputs("Operator: ",stdout);
	scanf("%c", &opmsg[opnd_cnt*OPSZ+1]);
	write(sock, opmsg, opnd_cnt*OPSZ+2);
	read(sock, &result, RLT_SIZE);
	
	printf("Operation result: %d \n",result);
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
	fputc('\n',stderr);
	exit(1);
}		/* -----  end of function error_handling  ----- */











