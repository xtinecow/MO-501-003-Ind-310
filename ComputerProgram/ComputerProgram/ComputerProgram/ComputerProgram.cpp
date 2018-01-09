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
	cout << "Hello World!" << endl; 

	if (argc == 1)
	{
		cout << "Error. No COMS port selected. Press 'E' to exit" << endl;
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


	int five = 5; 
	int seven = 7; 

	CSerial serial; 


}
