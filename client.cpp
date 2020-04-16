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
	printf("---------------------��������Ϸ�ͻ��ˣ�����h��ȡ������----------------------------\n");
	char times[5];
	recv(hSocket, times, 3, 0);
	getHelp();
	printf("������%c�β½����\n", times[0]);
	bool flag = true;//true=��Ϸδ������ false=��Ϸ����
	while(1){
		recv(hSocket, times, 3, 0);
		if(flag == false){
			send(hSocket, "end", 5, 0);
			return 0;
		}
		printf("������%c�λ���\n", times[0]);
		printf("���������²��4λ���֣�");
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
				ErrorHandling("���������֣�����h�鿴����\n");
			}else{
				send(hSocket, input, 5, 0);
				char result[5];
				recv(hSocket, result, 5, 0);
				printf("�����%s\n", result);
				if(strcmp(result,"Win!") == 0){
					printf("��ϲ���¶������֣�");
					break;
				}else if(times[0] == '1'){
					printf("�Բ��𣬲½���������꣡");
					flag = false;
				}
			}
		}else{
			ErrorHandling("������4λ���֣�����h�鿴����\n");
		}
	}
}
void ErrorHandling(char *message){
	fputs(message, stderr);
	fputc('\n',stderr);
	exit(1);
}
void getHelp(){
	printf("|��Ϸ����\n|����һ��4λ�����֣���ĳλ���ֺ�λ����ȷ����ʾΪA\n|��ĳλ������ȷ��λ�ô��󣬱�ʾΪB\n|���Ϊ1234��������3412����õ�0A4B,������1278����õ�2A0B\n");
}
void clean(){
	system("cls");
	printf("---------------------��������Ϸ�ͻ��ˣ�����h��ȡ������----------------------------\n");
}