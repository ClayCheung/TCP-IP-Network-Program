/*
 * =====================================================================================
 *
 *       Filename:  echo_server.c
 *
 *    Description:  socket option
 *					
 *					设置 REUSEADDR 参数 为 TRUE（1）
 *					使得 主动断开 socket 的 Host,
 *					在发出 四次握手的 最后一个ACK后，进入 Time-wait 的那段时间里，
 *					能够分配 本次会话的套接字的port 给新的套接字，
 *					REUSEADDR 参数 默认是 FALSE（0），
 *					在FALSE 情况下，Time-wait阶段，是不能把port分配给新的套接字的
 *
 *					这个程序把 REUSEADDR参数 设置为 TRUE（1），
 *					按ctrl+c 退出server程序后，可以立即再开启一个server ，port和关闭的server一样
 *
 *					× 注意不要按成 CTRL-Z 了
 *
 *					提问:CTRL-Z和CTRL-C区别?
 *					回答:
 *					CTRL-Z和CTRL-C都是中断命令,但是他们的作用却不一样. 
 *					CTRL-C是强制中断程序的执行,
 *					而CTRL-Z的是将任务中断,但是此任务并没有结束,他仍然在进程中他只是维持挂起的状态,
 *					用户可以使用fg/bg操作继续前台或后台的任务,fg命令重新启动前台被中断的任务,bg命令把被中断的任务放在后台执行.
 *					
 *					
 *        Version:  1.46
 *        Created:  2016年10月19日 14时36分01秒
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

#define TRUE 1
#define FALSE 0


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
	
	int option;


	struct sockaddr_in serv_adr;
	struct sockaddr_in clnt_adr;

	socklen_t clnt_adr_sz,
			  optlen;

	if(argc != 2){
		printf("Usage : %s <port>\n",argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
		error_handling("socket() error");

	option = TRUE;
	optlen = sizeof(option);
	if((setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&option, optlen)) == 1)
		error_handling("setsockopt() error");

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













