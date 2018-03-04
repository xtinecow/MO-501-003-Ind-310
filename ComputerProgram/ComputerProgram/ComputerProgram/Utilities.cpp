#include"ComputerProgram.h"
using namespace std;

void ParseNDResponse(char *response, int size)
{
	int node, macByte, numNodes, macIndex; 
		while (response[0] != 'F' && size>0)
	{
		response++; // Increment pointer when start of data is off
		size--; // Discarding first byte so size has to be decreased accordingly
		cout << "Aligning data..." << endl;
	}
	numNodes = size / ND_RESPONSE_SIZE;
	if (numNodes > MAX_NUM_NODES - 1)
	{
		// Should never happen since this does not return computer node (node 0)
		cout << "Error. Network Discover returned too many nodes" << endl; 
		return; 
	}
	for (node = 0; node<numNodes; node++)
	{
		for (macByte = 0; macByte<4; macByte++)
		{
			macIndex = node*ND_RESPONSE_SIZE + 5 + 2*macByte; // Offset of 5 for FFFE\r
			// Update Starting from node 1 (node 0 updated by find neighbors)
			NodeList[node+1].MAC[macByte] = (unsigned char)ConvertHexByteToInt(&response[macIndex]);
		}
		for (macByte = 0; macByte<4; macByte++)
		{
			macIndex = node*ND_RESPONSE_SIZE + 14 + 2*macByte; // Skip carriage return in middle of MAC
			NodeList[node+1].MAC[4 + macByte] = (unsigned char)ConvertHexByteToInt(&response[macIndex]);
		}
	}

	// Set remaining nodes to 0
	for (node = numNodes; node<MAX_NUM_NODES-1; node++)
	{
		for (macByte = 0; macByte<8; macByte++)
			NodeList[node + 1].MAC[macByte] = 0;
	}
}

// Same as Parse ND response but stores results for node 0's
// Also actually cares about RSSI
void ParseFNResponse(char *response, int size)
{
	int node, macByte, numNodes, macIndex;
	while (response[0] != 'F' && size>0)
	{
		response++; // Increment pointer when start of data is off
		size--; // Discarding first byte so size has to be decreased accordingly
		cout << "Aligning data... " << response[0] << endl;
	}

	numNodes = size / ND_RESPONSE_SIZE;
	for (node = 0; node<numNodes; node++)
	{
		for (macByte = 0; macByte<4; macByte++)
		{
			macIndex = node*ND_RESPONSE_SIZE + 5 + 2 * macByte; // Offset of 5 for FFFE\r
			NodeList[0].NodeTable[node].MAC[macByte] = (unsigned char)ConvertHexByteToInt(&response[macIndex]);
		}
		for (macByte = 0; macByte<4; macByte++)
		{
			macIndex = node*ND_RESPONSE_SIZE + 14 + 2 * macByte; // Skip carriage return in middle of MAC
			NodeList[0].NodeTable[node].MAC[4 + macByte] = (unsigned char)ConvertHexByteToInt(&response[macIndex]);
			NodeList[0].NodeTable[node].RSSI = (short)ConvertHexByteToInt(&response[node*ND_RESPONSE_SIZE + 46]);
		}
	}

	// Set remaining nodes to 0
	for (node = numNodes; node<MAX_NUM_NODES-1; node++)
	{
		for (macByte = 0; macByte<8; macByte++)
			NodeList[0].NodeTable[node].MAC[macByte] = 0;
		NodeList[0].NodeTable[node].RSSI = 0;
	}
}

// Take the 2 chars of HEX value and convert it to an int
int ConvertHexByteToInt(char* pointer)
{
	int i, dig;
	int sum = 0;

	for (i = 0; i<2; i++)
	{
		switch (pointer[i])
		{
		case '0': dig = 0;
			break;
		case '1': dig = 1;
			break;
		case '2': dig = 2;
			break;
		case '3': dig = 3;
			break;
		case '4': dig = 4;
			break;
		case '5': dig = 5;
			break;
		case '6': dig = 6;
			break;
		case '7': dig = 7;
			break;
		case '8': dig = 8;
			break;
		case '9': dig = 9;
			break;
		case 'A': dig = 10;
			break;
		case 'B': dig = 11;
			break;
		case 'C': dig = 12;
			break;
		case 'D': dig = 13;
			break;
		case 'E': dig = 14;
			break;
		case 'F': dig = 15;
			break;

		default:
			cout << "Invalid character " << pointer[i] << " entered" << endl;
			break;

		}
		if (!i) // First digit
			sum += 16 * dig;
		else
			sum += dig;
	}
	return sum;
}

void DisplayNodeList(void)
{
	int i, node, neighbor;

	ClearScreen(); 
	for (node = 0; node < MAX_NUM_NODES; node++)
	{
		cout << "Node number: " << node << endl;
		cout << "MAC: ";
		// Switch to hex format and print addresses 
		for (i = 0; i < 7; i++)
			cout << hex << uppercase << setw(2) << setfill('0') << (int)NodeList[node].MAC[i] << ":";
		cout << hex << uppercase << setw(2) << setfill('0') << (int)NodeList[node].MAC[7] << endl;
		for (neighbor = 0; neighbor < MAX_NUM_NODES-1; neighbor++)
		{
			cout << "	Node number: " << neighbor << "		";
			cout << "	MAC: ";
			for (i = 0; i < 7; i++)
				cout << hex << uppercase << setw(2) << setfill('0') << (int)NodeList[node].NodeTable[neighbor].MAC[i] << ":";
			// Print last one and switch back to dec
			cout << hex << uppercase << setw(2) << setfill('0') << (int)NodeList[node].NodeTable[neighbor].MAC[7] << dec << "	";
			cout << "	RSSI: " << NodeList[node].NodeTable[neighbor].RSSI << endl; 
		}
	}
}

// Checksum is 0xFF - sum of all bytes from API frame type through payload
void CalculateRequestChecksum(TxFrame *request)
{
	int i; 
	unsigned char checksum = 0xFF; 
	checksum -= request->type; 
	checksum -= request->ID;
	checksum -= request->FFFE[0];
	checksum -= request->FFFE[1];
	checksum -= request->broadcast; 
	checksum -= request->option;

	for (i=0; i<8; i++)
		checksum-= request->MAC[i];
	for (i=0; i<sizeof(request->payload); i++)
		checksum -= request->payload[i];

	// Append result back to struct
	request->checksum = checksum; 
}

// Grabs size bytes from source and copy each one into 2 bytes in dest. 
// First digit of source is first byte of dest, second digit is second byte
// Necessary for MAC transfer because serial class does not support unsigned char
void SplitByteArray(unsigned char *src, unsigned char* dest, int size)
{
	int i; 
	for (i = 0; i < size; i++)
	{
		dest[2*i] = (src[i] & 0xf0) >> 4; 
		dest[2 * i + 1] = (src[i] & 0x0f);
	}
}

// Undo changes done by SplitByteArray
// Combine src of size 2*size back into dest of size size
void CombineByteArray(unsigned char *src, unsigned char* dest, int size)
{
	int i;
	for (i = 0; i<size; i++)
	{
		dest[i] = (src[2 * i] << 4) + src[2 * i + 1];
	}
}

// Functioned copied from http://www.cplusplus.com/articles/4z18T05o/
void ClearScreen(void)
{
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X *csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(
		hStdOut,
		(TCHAR) ' ',
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(
		hStdOut,
		csbi.wAttributes,
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Move the cursor home */
	SetConsoleCursorPosition(hStdOut, homeCoords);
}

// Compare MAC against known nodes and see if it matches any
string GetNameFromMAC(unsigned char* MAC)
{
	string result;

	// Check for 0 (no node connected)
	if (MAC[0] == 0 && MAC[1] == 0 && MAC[2] == 0 && MAC[3] == 0 && MAC[4] == 0 && MAC[5] == 0 && MAC[6] == 0 && MAC[7] == 0)
		return "NoNode"; 

	// First half should always be the same
	if (MAC[0] != 0x00 || MAC[1] != 0x13 || MAC[2] != 0XA2 || MAC[3] != 0X00)
		return "Unknown";

	if (MAC[4] == 0x41 && MAC[5] == 0x05 && MAC[6] == 0xE6 && MAC[7] == 0x14)
		return "Node0";

	if (MAC[4] == 0x41 && MAC[5] == 0x0A && MAC[6] == 0x3A && MAC[7] == 0x93)
		return "Node1";

	if (MAC[4] == 0x41 && MAC[5] == 0x05 && MAC[6] == 0xE6 && MAC[7] == 0x0E)
		return "Node2";

	if (MAC[4] == 0x41 && MAC[5] == 0x0A && MAC[6] == 0x3A && MAC[7] == 0x45)
		return "Node3";

	if (MAC[4] == 0x41 && MAC[5] == 0x05 && MAC[6] == 0xE6 && MAC[7] == 0x0D)
		return "Node4";

	if (MAC[4] == 0x41 && MAC[5] == 0x0A && MAC[6] == 0x3A && MAC[7] == 0x99)
		return "Node5";

	return "Unknown";
}
