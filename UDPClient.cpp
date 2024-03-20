#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <stdlib.h>

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

// Because client is not binding to server (connect), 
// server address need to be specified in sendto()
// 
// int sendto(
//   _In_       SOCKET s,
//   _In_ const char   *buf,
//   _In_       int    len,
//   _In_       int    flags,
//   _In_ const struct sockaddr *to,
//   _In_       int    tolen
// );

int main()
{
	int iResult{ 0 };
	WSADATA wsaData;
	SOCKET SendSocket;
	sockaddr_in RecvAddr;
	unsigned short Port = 27015;
	char RecvBuf[1024];
	int BufLen = 1024;
	sockaddr_in SenderAddr;
	int SenderAddrSize = sizeof(SenderAddr);

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	// Create a socket for sending data
	SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (SendSocket == INVALID_SOCKET)
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	// Set up the RecvAddr structure with the IP address of
	// the receiver and the specified port number.
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(Port);
	inet_pton(AF_INET, "192.168.42.124", (void*)&RecvAddr.sin_addr.S_un.S_addr);

	// Send a datagram to the receiver
	wprintf(L"Sending a datagram to the receiver...\n");
	iResult = sendto(SendSocket, "This is a test", 13, 0, (SOCKADDR*)&RecvAddr, sizeof(RecvAddr));
	if (iResult == SOCKET_ERROR)
	{
		wprintf(L"sendto failed with error: %d\n", WSAGetLastError());
		closesocket(SendSocket);
		WSACleanup();
		return 1;
	}

	// When the application is finished sending, close the socket.
	wprintf(L"Finished sending. Closing socket.\n");
	iResult = closesocket(SendSocket);
	if (iResult == SOCKET_ERROR)
	{
		wprintf(L"closesocket failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	// Clean up and exit
	wprintf(L"Exiting.\n");
	WSACleanup();
	
	system("pause");
	return 0;
}