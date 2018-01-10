// ComputerProgram.cpp
// This will be the main file for the computer program. 
// This program will be responsible for finding all nodes within the network and pulling them for their respective tables




#include "ComputerProgram.h"
using namespace std; 

// Input arguments will be as follows:
// 0: COM Port 
int main(int argc, char *argv[])
{
	char keyboardCommand; 
	int portNum; 
	int i; 
	CSerial serial;

	cout << "Please enter COMS port number of XBee Module." << endl;
	cin >> portNum; 


	while (portNum < 0 || portNum > 255)
	{
		cout << "Invalid port number entered. Please select a number between 0 and 255" << endl;
		cin >> portNum;
	}

	if (serial.Open(portNum, XBEE_BAUDRATE))
		cout << "Port successfully opened";
	else
		cout << "Failed to open port " << portNum << endl; 

	//string num = to_string(50);
	//char sizePort[15];
	//string temp;
	//temp = "COMS" + num;
	//for (i = 0; i < size(sizePort); i++)
	//{
	//	sizePort[i] = temp[i]; 
	//	if (temp[i] == 0)
	//		break; 
	//}
	//cout << "String is " << sizePort; 



		while (1)
		{
			while (!(keyboardCommand = CheckKeyboard())) {}; // Wait for input
			if (keyboardCommand == 'E')
				exit(1);
			else
			{
				cout << keyboardCommand << endl; // Just spit out command if not E
				keyboardCommand = 0;
				Sleep(100); // Delay for a bit
			}
		}


}
