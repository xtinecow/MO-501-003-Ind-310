// cppClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include "stdafx.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Iphlpapi.h>
#include <stdio.h>
#include <string>
#include <chrono>
#include <thread>


#pragma comment(lib, "Ws2_32.lib")
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#define IPADD "192.168.1.68"
#define DEFAULT_PORT "8001"
#define DEFAULT_BUFLEN 1024

using namespace std;
int main() 
{
	#define SOCKETSTARTUP
	#ifdef SOCKETSTARTUP
	WSADATA wsaData;

	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(IPADD, DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	SOCKET ConnectSocket = INVALID_SOCKET;

	// Attempt to connect to the first address returned by
	// the call to getaddrinfo
	ptr = result;

	// Create a SOCKET for connecting to server
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,ptr->ai_protocol);

	if (ConnectSocket == INVALID_SOCKET) 
	{
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}


	// Connect to server.
	iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	}

	
	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET)
	{
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}
	#endif

	#define SOCKETCOMS
	#ifdef SOCKETCOMS
	int recvbuflen = DEFAULT_BUFLEN;

	char *sendbuf;
	char recvbuf[DEFAULT_BUFLEN];
		
	
	string ID = "UofC";
	string ID2 = "test1";
	string ID3 = "test2";
	double lat = 51.079948;
	double longi = -114.125534;
	double er1 = 10;
	double er2 = 10;

	int whichID = 0;


	
	do {

		string str = ID + "," + to_string(lat) + "," + to_string(longi) + "," + to_string(er1) + "," + to_string(er2);
		
		switch (whichID)
		{
		case 0:
			str = ID + "," + to_string(lat) + "," + to_string(longi) + "," + to_string(er1) + "," + to_string(er2);
			er1 = er1 + 0.2;
			er2 = er2 + 0.1;
			whichID = 1;
			break;

		case 1:
			str = ID2 + "," + to_string(lat + 0.00020) + "," + to_string(longi + 0.0003) + "," + to_string(er1 - 3) + "," + to_string(er2 - 3);
			er1 += 0.1;
			er2 += 0.2;
			whichID = 2;
			break;
		case 2:
			str = ID3 + "," + to_string(lat - 0.00020) + "," + to_string(longi - 0.0003) + "," + to_string(er1 + 2) + "," + to_string(er2 + 3);
			whichID = 0;
			er1 += 0.1;
			er2 += 0.1;
			break;
		}

		
		sendbuf = _strdup(str.c_str());
		
		

		iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
		if (iResult == SOCKET_ERROR) {
			printf("send failed: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			return 1;
		}

		
		free(sendbuf);

		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
			printf("Bytes received: %d\n", iResult);
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed: %d\n", WSAGetLastError());


		lat += 00.000001;
		longi += -0.00001;


		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		if (lat > 180) break;
		
	} while (iResult > 0);


	string endMessage = "DONE";
	sendbuf = _strdup(endMessage.c_str());
	
	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	free(sendbuf);

	// shutdown the connection for sending since no more data will be sent
	// the client can still use the ConnectSocket for receiving data
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	// shutdown the send half of the connection since no more data will be sent
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();
	#endif

	return 0;
}