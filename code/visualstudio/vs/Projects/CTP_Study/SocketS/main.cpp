//[http://blog.sina.com.cn/s/blog_a40b5deb0101cfky.html]
//Server.cpp
#include <iostream>
#include <Windows.h>
#include <cmath>

using namespace std;

const  int PORT = 4000;
#define IP_ADDRESS "10.20.72.54"
#pragma comment(lib,"ws2_32.lib")

DWORD WINAPI ClientRecvThread(LPVOID lpParameter)
{
	SOCKET ClientSocket = (SOCKET)lpParameter;
	int iRet = 0;
	char chRecvBuffer[MAX_PATH];
	while(true)
	{
		memset(chRecvBuffer, 0x00, sizeof(chRecvBuffer));
		iRet = recv(ClientSocket, chRecvBuffer, MAX_PATH, 0);
		if(iRet == 0 || iRet== SOCKET_ERROR)
		{
			cout<< "接收消息错误！" << endl;
			continue;
		}
		cout<< "接收的信息为：" << chRecvBuffer << endl;
	}

	return 0;
}

//这个线程处理发送的数据
DWORD WINAPI ClientSendThread(LPVOID lpParameter)
{
	SOCKET ClientSocket = (SOCKET)lpParameter;
	int iRet = 0;
	char chSendBuffer[MAX_PATH];
	while(true)
	{
		memset(chSendBuffer, 0x00, sizeof(chSendBuffer));
		cin.getline(chSendBuffer, sizeof(chSendBuffer));
		iRet = send(ClientSocket, chSendBuffer, (int)strlen(chSendBuffer), 0);
		if(iRet == SOCKET_ERROR)
		{
			cout<< "Server::Send Info Error!" << endl;
			break;
		}
	}

	return 0;
}

SOCKET g_sAllSocket[8] = {INVALID_SOCKET};

//这个线程处理发送的数据
DWORD WINAPI ClientRandomSendThread(LPVOID lpParameter)
{
	int iRet = 0;
	char chSendBuffer[MAX_PATH];
	int nCount = 0;
	FD_SET writeSet;
	while(true)
	{
		nCount++;
		Sleep(3000);
		//if (nCount%10000 == 0) {
			memset(chSendBuffer, 0x00, sizeof(chSendBuffer));
			//cin.getline(chSendBuffer, sizeof(chSendBuffer));
			sprintf(chSendBuffer, "test:%d", (nCount));
			//strcpy(chSendBuffer, sprintf("test:%d", (nCount/1000)));
			
			int nIdx = rand()%(sizeof(g_sAllSocket)/sizeof(g_sAllSocket[0]));
			SOCKET RandomClientSocket = g_sAllSocket[nIdx];
			FD_ZERO(&writeSet);
			FD_SET(RandomClientSocket, &writeSet);
			int nRet = select(0, NULL, &writeSet, NULL, NULL);
			printf("Random(%d): %d, %d, %d\n", nCount, nIdx, (int)RandomClientSocket, FD_ISSET(RandomClientSocket, &writeSet));
			if (nRet > 0 )
			{
				if (RandomClientSocket != INVALID_SOCKET && FD_ISSET(RandomClientSocket, &writeSet))
					iRet = send(RandomClientSocket, chSendBuffer, (int)strlen(chSendBuffer), 0);
			}
		//}
	}

	return 0;
}

int main(int argc, char* argv[])
{
	WSADATA Ws ;
	SOCKET ServerSocket, ClientSocket;
	struct sockaddr_in LocalAddr, ClientAddr;
	int iRet = 0;
	int AddrLen = 0;
	HANDLE hThread = NULL;

	//Init windows socket
	if(WSAStartup(MAKEWORD(2,2), &Ws) != 0)
	{
		cout<< "Init Windows Socket Failed::" <<GetLastError() << endl;
		return -1;
	}

	//Create Socket
	ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(ServerSocket== INVALID_SOCKET)
	{
		cout<< "Create Socket Failed::" << GetLastError() << endl;
		return -1;
	}


	LocalAddr.sin_family = AF_INET;
	LocalAddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
	LocalAddr.sin_port = htons(PORT);
	memset(LocalAddr.sin_zero, 0x00, 8);



	//Bind Socket
	iRet = bind(ServerSocket, (struct sockaddr*)&LocalAddr, sizeof(LocalAddr));
	if(iRet != 0)
	{
		cout<< "Bind Socket Failed::" << GetLastError() << endl;
		return -1;
	}

	iRet = listen(ServerSocket, 10);
	if(iRet != 0)
	{
		cout<< "Listen Socket Failed !" << GetLastError() << endl;
		return -1;
	}

	cout << "服务端已经启动！" << endl;

	hThread = CreateThread(NULL, 0, ClientRandomSendThread, 0, 0, NULL);
	if(hThread == NULL)
	{
		cout<< "Create RandomSendThread Failed!" << endl;
		return -1;
	}
	CloseHandle(hThread);

	int g_nSocket = 0;
	while(true)
	{
		AddrLen = sizeof(ClientAddr);
		ClientSocket= accept(ServerSocket, (struct sockaddr*)&ClientAddr, &AddrLen);
		if(ClientSocket== INVALID_SOCKET)
		{
			cout<< "Accept Failed::" << GetLastError() << endl;
			return -1;
		}
		g_sAllSocket[g_nSocket++] = ClientSocket;

		cout<<"客户端连接::" << inet_ntoa(ClientAddr.sin_addr) << ":"<< ClientAddr.sin_port << endl;
		hThread = CreateThread(NULL, 0, ClientRecvThread, (LPVOID)ClientSocket, 0, NULL);
		if(hThread == NULL)
		{
			cout<< "Create Thread Failed!" << endl;
			return -1;
		}
		CloseHandle(hThread);

		hThread = CreateThread(NULL, 0, ClientSendThread, (LPVOID)ClientSocket, 0, NULL);
		if(hThread == NULL)
		{
			cout<< "Create Thread Failed!" << endl;
			return -1;
		}
		CloseHandle(hThread);
	}

	closesocket(ServerSocket);
	closesocket(ClientSocket);
	WSACleanup();

	return 0;
}
//////////////////////////////////////////////////////////////////////////
//TCP server
/*
listen port 9102
receive string and display it

Visual C++ 6.0

#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib")


#define BUFLEN 1024

int main()
{
	SOCKET serversoc;
	SOCKET clientsoc;
	SOCKADDR_IN serveraddr;
	SOCKADDR_IN clientaddr;
	char buf[BUFLEN];
	int len;

	WSADATA wsa;
	WSAStartup(MAKEWORD(1,1),&wsa);	//initial Ws2_32.dll by a process
	if((serversoc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) <= 0)	//create a tcp socket
	{
		printf("Create socket fail!\n");
		return -1;
	}

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9102);
	serveraddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	if(bind(serversoc, (SOCKADDR *)&serveraddr, sizeof(serveraddr)) != 0)
	{
		printf("Bind fail!\n");
		return -1;
	}

	//start listen, maximum length of the queue of pending connections is 1
	printf("Start listen...\n");
	if(listen(serversoc, 1) != 0)
	{
		printf("Listen fail!\n");
		return -1;
	}

	len = sizeof(SOCKADDR_IN);


	//waiting for connecting
	if((clientsoc = accept(serversoc, (SOCKADDR *)&clientaddr, &len))<=0)
	{
		printf("Accept fail!\n");
		return -1;
	}
	printf("Connected\n");
	while(1)
	{
		//waiting for data receive
		if(recv(clientsoc, buf, BUFLEN, 0) <= 0)	
		{
			//some error occur
			printf("Close connection\n");
			closesocket(clientsoc);
			break;
		}
		printf("%s\n",buf);
	}

	WSACleanup(); //clean up Ws2_32.dll 
	return 0;
}
*/

// #include <stdlib.h>
// #include <stdio.h>
// #include <errno.h>
// #include <string.h>
// #include <unistd.h>
// #include <netdb.h>
// 
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <sys/types.h>
// #include <arpa/inet.h>
// 
// #define MY_PORT         9102
// 
// int main(int argc ,char **argv)
// {
// 	int listen_fd,accept_fd;
// 	struct sockaddr_in     client_addr;
// 	int n;

// 	if((listen_fd=socket(AF_INET,SOCK_STREAM,0))<0)
// 	{
// 		printf("Socket Error:%s\n\a",strerror(errno));
// 		exit(1);
// 	}
// 
// 	bzero(&client_addr,sizeof(struct sockaddr_in));
// 	client_addr.sin_family=AF_INET;
// 	client_addr.sin_port=htons(MY_PORT);
// 	client_addr.sin_addr.s_addr=htonl(INADDR_ANY);
// 	n=1;
// 	/* 如果服务器终止后,服务器可以第二次快速启动而不用等待一段时间  */
// 	setsockopt(listen_fd,SOL_SOCKET,SO_REUSEADDR,&n,sizeof(int));
// 	if(bind(listen_fd,(struct sockaddr *)&client_addr,sizeof(client_addr))<0)
// 	{
// 		printf("Bind Error:%s\n\a",strerror(errno));
// 		exit(1);
// 	}
// 	listen(listen_fd,5);
// 	while(1)
// 	{
// 		accept_fd=accept(listen_fd,NULL,NULL);
// 		if((accept_fd<0)&&(errno==EINTR))
// 			continue;
// 		else if(accept_fd<0)
// 		{
// 			printf("Accept Error:%s\n\a",strerror(errno));
// 			continue;
// 		}
// 		if((n=fork())==0)
// 		{
// 			/* 子进程处理客户端的连接 */
// 			char buffer[1024];
// 
// 			close(listen_fd);
// 			n=read(accept_fd,buffer,1024);
// 			write(accept_fd,buffer,n);
// 			close(accept_fd);
// 			exit(0);
// 		}
// 		else if(n<0)
// 			printf("Fork Error:%s\n\a",strerror(errno));
// 		close(accept_fd);
// 	}
// } 