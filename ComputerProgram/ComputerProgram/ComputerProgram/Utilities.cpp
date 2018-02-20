#include"ComputerProgram.h"
using namespace std;

void ParseNDResponse(char *response, int size)
{
	int node, macByte, numNodes, macIndex; 

	numNodes = size / ND_RESPONSE_SIZE;
	for (node = 0; node<numNodes; node++)
	{
		for (macByte = 0; macByte<4; macByte++)
		{
			macIndex = node*ND_RESPONSE_SIZE + 5 + 2*macByte; // Offset of 5 for FFFE\r
			NodeList[node].MAC[macByte] = (unsigned char)ConvertHexByteToInt(&response[macIndex]);
		}
		for (macByte = 0; macByte<4; macByte++)
		{
			macIndex = node*ND_RESPONSE_SIZE + 14 + 2*macByte; // Skip carriage return in middle of MAC
			NodeList[node].MAC[4 + macByte] = (unsigned char)ConvertHexByteToInt(&response[macIndex]);
		}
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

	for (node = 0; node < MAX_NUM_NODES; node++)
	{
		cout << "Node number: " << node << endl;
		cout << "MAC: ";
		// Switch to hex format and print addresses 
		for (i = 0; i < 7; i++)
			cout << hex << uppercase << setw(2) << setfill('0') << (int)NodeList[node].MAC[i] << ":";
		cout << hex << uppercase << setw(2) << setfill('0') << (int)NodeList[node].MAC[7] << endl;
		for (neighbor = 0; neighbor < MAX_NUM_NODES; neighbor++)
		{
			cout << "	Node number: " << neighbor << endl;
			cout << "	MAC: ";
			for (i = 0; i < 7; i++)
				cout << hex << uppercase << setw(2) << setfill('0') << (int)NodeList[node].NodeTable[neighbor].MAC[i] << ":";
			// Print last one and switch back to dec
			cout << hex << uppercase << setw(2) << setfill('0') << (int)NodeList[node].NodeTable[neighbor].MAC[7] << dec << endl;
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