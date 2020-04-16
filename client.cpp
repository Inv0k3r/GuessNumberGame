#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <Windows.h>
void ErrorHandling(char *message);
void getHelp();
void clean();
int main(int argc, char * argv[]){
	argc = 3;
	argv[0] = "";
	argv[1] = "127.0.0.1";
	argv[2] = "9190";
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;
	if(argc != 3){
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}
	if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
		ErrorHandling("WSAStartup() Error!");
	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if(hSocket == INVALID_SOCKET)
		ErrorHandling("socket() Error!");
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));
	if(connect(hSocket,(SOCKADDR *)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() Error!");
	printf("---------------------猜数字游戏客户端（输入h获取帮助）----------------------------\n");
	char times[5];
	recv(hSocket, times, 3, 0);
	getHelp();
	printf("您共有%c次猜解机会\n", times[0]);
	bool flag = true;//true=游戏未结束， false=游戏结束
	while(1){
		recv(hSocket, times, 3, 0);
		if(flag == false){
			send(hSocket, "end", 5, 0);
			return 0;
		}
		printf("您还有%c次机会\n", times[0]);
		printf("请输入您猜测的4位数字：");
		char input[5];
		scanf("%s",&input);
		if(strlen(input) == 1){
			if(input[0] == 'h'){
				clean();
				getHelp();
			}
		}else if(strlen(input) == 4){
			int i = 0;
			for(i = 0; i < 4; i ++){
				if(!(input[i] - '0' >= 0 && input[i] - '0' <= 9)){
					break;
				}
			}
			if(i != 4){
				ErrorHandling("请输入数字，输入h查看帮助\n");
			}else{
				send(hSocket, input, 5, 0);
				char result[5];
				recv(hSocket, result, 5, 0);
				printf("结果：%s\n", result);
				if(strcmp(result,"Win!") == 0){
					printf("恭喜您猜对了数字！");
					break;
				}else if(times[0] == '1'){
					printf("对不起，猜解次数已用完！");
					flag = false;
				}
			}
		}else{
			ErrorHandling("请输入4位数字，输入h查看帮助\n");
		}
	}
}
void ErrorHandling(char *message){
	fputs(message, stderr);
	fputc('\n',stderr);
	exit(1);
}
void getHelp(){
	printf("|游戏规则：\n|输入一个4位的数字，若某位数字和位置正确，表示为A\n|若某位数字正确而位置错误，表示为B\n|如答案为1234，若输入3412，则得到0A4B,若输入1278，则得到2A0B\n");
}
void clean(){
	system("cls");
	printf("---------------------猜数字游戏客户端（输入h获取帮助）----------------------------\n");
}