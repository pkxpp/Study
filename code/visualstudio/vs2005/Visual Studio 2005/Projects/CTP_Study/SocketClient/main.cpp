//client
#include <iostream>
#include <windows.h>

using namespace std;

#define PORT 4000
#define IP_ADDRESS "10.20.72.54"

#pragma comment(lib,"ws2_32.lib")

//这个线程处理接收的数据
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
		cout<< "接收到信息为：" << chRecvBuffer << endl;
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
int main(int argc, char* argv[])
{
	WSADATA Ws;
	SOCKET ClientSocket;
	struct sockaddr_in ServerAddr;
	int iRet = 0;
	int iAddrLen = 0;
	HANDLE hThread = NULL;

	//init windows socket;
	if(WSAStartup(MAKEWORD(2,2), &Ws) != 0 )
	{
		cout<< "init windows socket failed::" << GetLastError() << endl;
		return -1;
	}

	//Create Socket
	ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(ClientSocket== INVALID_SOCKET)
	{
		cout<< "create socket failed::" << GetLastError() << endl;
		return -1;
	}

	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
	ServerAddr.sin_port = htons(PORT);
	memset(ServerAddr.sin_zero, 0x00, 8);

	//Connect Server
	iRet = connect(ClientSocket, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr));
	if(iRet == SOCKET_ERROR)
	{
		cout<< "Connect Error::" <<GetLastError() << endl;
		return -1;
	}
	else
	{
		cout<< "Connect successful " <<endl;
	}

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
	WaitForSingleObject(hThread, 100000000);
	CloseHandle(hThread);

	closesocket(ClientSocket);
	WSACleanup();
	return 0;

}


//////////////////////////////////////////////////////////////////////////

//TCP client
//client send string to server

//Visual C++ 6.0

// #include <stdio.h>
// #include <winsock2.h>
// 
// #pragma comment(lib,"ws2_32.lib")
// 
// 
// int main()
// {
// 	SOCKET soc;
// 	SOCKADDR_IN serveraddr;
// 	SOCKADDR_IN clientaddr;
// 	char buf[1024];
// 
// 	WSADATA wsa;
// 	WSAStartup(MAKEWORD(1,1),&wsa);	//initial Ws2_32.dll by a process
// 
// 	if((soc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) <= 0)	//create a tcp socket
// 	{
// 		printf("Create socket fail!\n");
// 		return -1;
// 	}
// 
// 	serveraddr.sin_family = AF_INET;
// 	serveraddr.sin_port = htons(9102);
// 	serveraddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
// 
// 	//connect to server
// 	printf("Try to connect...\n");
// 	if(connect(soc, (SOCKADDR *)&serveraddr, sizeof(serveraddr)) != 0)
// 	{
// 		printf("Connect fail!\n");
// 		return -1;
// 	}
// 	printf("Connected\n");
// 	while(1)
// 	{
// 		scanf("%s", buf);
// 		//send to server
// 		if(send(soc, buf, strlen(buf)+1, 0)<=0)
// 		{
// 			printf("Error!\n");
// 		}
// 	}
// 
// 	WSACleanup(); //clean up Ws2_32.dll 
// 	return 0;
// }