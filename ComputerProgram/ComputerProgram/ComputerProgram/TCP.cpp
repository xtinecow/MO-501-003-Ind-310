#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "ComputerProgram.h"


#include <winsock2.h>
#include <ws2tcpip.h>
#include <Iphlpapi.h>
#include <stdio.h>
#include <sstream>


#pragma comment(lib, "Ws2_32.lib")

#define IPADD "10.13.183.68"
#define DEFAULT_PORT "8002"
#define DEFAULT_BUFLEN 1024

using namespace std;

WSADATA wsaData;
int iResult;
SOCKET ListenSocket;
SOCKET ClientSocket;


int SetupTCPSocket(void)
{
	cout << "Setting up socket... "; 
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return 0;
	}

	struct addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return 0;
	}


	ListenSocket = INVALID_SOCKET;

	// Create a SOCKET for the server to listen for client connections

	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (ListenSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 0;
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 0;
	}

	freeaddrinfo(result);





	if (listen(ListenSocket, 1) == SOCKET_ERROR) {
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 0;
	}

	ClientSocket = INVALID_SOCKET;

	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 0;
	}

	// Socket successfully setup 
	cout << "done." << endl; 
	return 1; 
}

void SendNodesToSocket(void)
{
	int node, neighbor; 
	 char* NodePointer; 
	 /*char test[3]; 
	 test[0] = 1; 
	 test[1] = 0; 
	 test[2] = 2; */

	 // Have to convert to string because TCP socket doesn't let you send 0's
	 string temp; 
	 for (node = 0; node < MAX_NUM_NODES; node++)
	 {
		 temp += GetNameFromMAC(NodeList[node].MAC); 
		 temp += "-"; // Parsing character
		 for (neighbor = 0; neighbor < MAX_NUM_NODES - 1; neighbor++)
		 {
			 temp += GetNameFromMAC(NodeList[node].NodeTable[neighbor].MAC);
			 temp += "."; // Parsing character
			 temp += to_string(NodeList[node].NodeTable[neighbor].RSSI);
			 temp += "|"; // Parsing character
		 }
		 temp += ">"; 
	 }
	 temp += ":"; // Terminating character

	NodePointer = _strdup(temp.c_str());

	cout << "Sending node info to socket...";
	// Send to socket
	iResult = send(ClientSocket, NodePointer, (int)strlen(NodePointer), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
	}
	cout << "done." << endl; 
}


void CloseTCPSocket(void)
{
	cout << "Closing TCP Socket... "; 
	// shutdown the send half of the connection since no more data will be sent
	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return;
	}

	closesocket(ClientSocket);
	WSACleanup();
	cout << "done." << endl; 
}