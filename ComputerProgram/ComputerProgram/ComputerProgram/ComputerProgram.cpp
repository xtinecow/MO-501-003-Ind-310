// ComputerProgram.cpp
// This will be the main file for the computer program. 
// This program will be responsible for finding all nodes within the network and pulling them for their respective tables


#include "ComputerProgram.h"
using namespace std; 
int setupComplete;
int globalErrorCount;

int main(int argc, char *argv[])
{
	int portNum; 
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
	 //if (!SetupTCPSocket())
		// WaitForExit();


	 SetATCommandMode(); 
	 ReadFirmwareVersion(); 
	 GetHostMAC(); 

	 SetNetworkID(); 
	 SetNetworkTimeout(); 
	 NetworkDiscover();

	 // Network discover makes command mode time out so need to set it again
	  SetATCommandMode();
	  SetAPIMode(); 
	  ExitCommandMode(); 
	  DisplayNodeList(); 
	  setupComplete = 1; 

	  // With setup complete, program can attempt to recover from errors by simply starting RunProgram again. 
		 RunProgram(); 
	  
	      // Old Keyboard support code
#if 0
		while (1)
		{
			while (!(keyboardCommand = CheckKeyboard())) {}; // Wait for input
			if (keyboardCommand == 'E')
			{
				exit(1);
				CloseSerialPort();
			}
			else
			{
				SendNodesToSocket(); 
				keyboardCommand = 0;
				Sleep(100); // Delay for a bit
			}
		}
#endif


}

#define NUM_STAGES 9
void RunProgram(void)
{
	int node, i, stage, networkCounter; 
	static int errCount = 0; 

	stage = 0; 
	networkCounter = 0; 
	while (1)
	{
		if (globalErrorCount > errCount)
		{
			cout << "Error with stage " << stage << " detected. Retrying..." << endl;
			errCount = globalErrorCount;
		}
		else
		{
			// Move to next stage if no error detected. 
			if (stage == NUM_STAGES)
				stage = 1; 
			else
				stage++; 
		}

		switch (stage)
		{
			case 1:
				SetATCommandMode();
				break; 

			case 2: 
				FindNeighbors();
				break; 

			case 3: 
				// Sleep(4000); // Give it some time to settle (not sure why but this seems to be needed) 
		 		SetATCommandMode();
				break; 

			case 4: 
				SetAPIMode();
				break; 

			case 5: 
				ExitCommandMode();
				break; 

			case 6:
				for (node = 1; node < MAX_NUM_NODES; node++)
				{
					// Check MAC address to see if there is a non-zero node
					for (i = 0; i < 8; i++)
					{
						if (NodeList[node].MAC[i] != 0)
						{
							SendTableRequest(node);
							WaitForTableFrame(node);
							break;
						}
					}
				}
				break; 

			case 7:
				DisplayNodeList();
				break; 

			case 8:
				// Upload to TCP socket for positioning algorithm
				SendNodesToSocket(); 
				break; 

			case 9: 
				// Update network every 10 cycles
				if (networkCounter < 10)
					networkCounter++; 
				else
				{
					SetATCommandMode(); 
					NetworkDiscover();
					networkCounter = 0; 
				}
				break; 

		}
	}
}