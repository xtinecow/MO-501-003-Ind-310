// ComputerProgram.cpp
// This will be the main file for the computer program. 
// This program will be responsible for finding all nodes within the network and pulling them for their respective tables


#include "ComputerProgram.h"
using namespace std; 

int main(int argc, char *argv[])
{
	int portNum, i;
	char keyboardCommand; 


	// Prompt user for port number
	cout << "Please enter COMS port number of XBee Module." << endl;
	cin >> portNum; 


	while (portNum < 0 || portNum > 255)
	{
		cout << "Invalid port number entered. Please select a number between 0 and 255" << endl;
		cin >> portNum;
	}

	 OpenSerialPort(portNum); 
	 SetATCommandMode(); 
	 ReadFirmwareVersion(); 

	 NetworkDiscover(); 

	 // Close port
	 CloseSerialPort();  
	 cout << "Node 0 has MAC: "; 
	 for (i = 0; i < 8; i++)
		 cout << NodeList[0].MAC[2 * i] << NodeList[0].MAC[2 * i + 1] << ":";
	 cout << endl; 

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
