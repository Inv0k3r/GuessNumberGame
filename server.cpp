#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <time.h>
void ErrorHandling(char *message);
char *check(char *input, char *result);
#define random rand()%9000 + 1000
int main(int argc, char * argv[]){
	argv[0] = "";
	argv[1] = "9190";
	argc = 2;
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr,clnAddr;
	int szClntAddr;
	if(argc != 2){
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}
	if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
		ErrorHandling("WSAStartup() Error!");
	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if(hServSock == INVALID_SOCKET)
		ErrorHandling("socket() Error!");
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));
	if(bind(hServSock, (SOCKADDR *)&servAddr,sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() Error!");
	if(listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() Error!");
	srand((unsigned) time(NULL));
	char choice[100];
	time_t t = time(0); 
	printf("------------猜数游戏服务器端----------------\n");
	szClntAddr = sizeof(clnAddr);
	while(1){
		hClntSock = accept(hServSock, (SOCKADDR*)&clnAddr, &szClntAddr);
		if(hClntSock == INVALID_SOCKET)
			ErrorHandling("accept() Error!");
		int result = random;//获得一个随机数
		printf("IP : [%s]\t Port: [%d] 已连接，分配的答案为：%d\n", inet_ntoa(clnAddr.sin_addr),clnAddr.sin_port,result);
		int times = 8;//默认8次
		//printf("输入猜解次数：");
		//scanf("%d",&times);
		char temp[3];
		temp[0] = '0' + times;
		if(send(hClntSock, temp, 3, 0)){
			for(int i = 0; i < times + 1; i ++){
				//发送剩余猜解次数
				temp[0] = char('0' + times - i);
				send(hClntSock, temp, 3, 0);
				char input[5];
				recv(hClntSock, input, 5, 0);
				if(strcmp(input, "end") == 0){
					printf("IP : [%s]\t Port: [%d] 猜解次数已用完，连接关闭\n", inet_ntoa(clnAddr.sin_addr),clnAddr.sin_port);
					break;
				}
				printf("IP : [%s]\t Port: [%d] 第 %d 次提交的答案为%s\n", inet_ntoa(clnAddr.sin_addr),clnAddr.sin_port,i + 1,input);
				char temp[5];
				temp[0] = '0' + result / 1000;
				temp[1] = '0' + (result % 1000) / 100;
				temp[2] = '0' + (result % 100) / 10;
				temp[3] = '0' + (result % 10);
				temp[4] = '\0';
				char *back = check(input,temp);
				if(back[0] == '4'){
					send(hClntSock, "Win!", 5, 0);
					printf("IP : [%s]\t Port: [%d] 成功猜到数字\n", inet_ntoa(clnAddr.sin_addr),clnAddr.sin_port);
					break;
				}
				else{
					send(hClntSock, back, 5, 0);
					printf("IP : [%s]\t Port: [%d] 返回的结果为：%s\n", inet_ntoa(clnAddr.sin_addr),clnAddr.sin_port,back);
				}
			}
		}
		closesocket(hClntSock);
	}
	closesocket(hServSock);
	WSACleanup();
	return 0;
}
void ErrorHandling(char *message){
	fputs(message, stderr);
	fputc('\n',stderr);
	exit(1);
}
char *check(char *input, char *result){
	int a = 0, b = 0;
	for(int i = 0; i < 4; i ++){
		for(int j = 0; j < 4; j ++){
			if(input[i] == result[j]){
				if(i == j){
					a ++;
				}else{
					b ++;
				}
			}
		}
	}
	char *temp = new char[5];
	temp[0] = a + '0';
	temp[1] = 'A';
	temp[2] = b + '0';
	temp[3] = 'B';
	temp[4] = '\0';
	return temp;
}