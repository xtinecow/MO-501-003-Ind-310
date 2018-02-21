#include "ComputerProgram.h"
// This file will include all keyboard commands that the user can enter to affect the program
using namespace std; 

char CommandList[] = {
'E', // Exit
'A' // Output an A
};
						
					
char CheckKeyboard(void)
{
	int i; 
	short keyState; 
	char command; 
	for (i = 0; i < sizeof(CommandList); i++) // Compare against all possible known commands
	{
		command = CommandList[i];
		keyState = GetKeyState(command);
		if (keyState & 0x8000) // Key Pressed
			return command;
	}
	return 0; // returns 0 if no keys are pressed
}

void WaitForExit(void)
{
	if (setupComplete)
	{
		globalErrorCount++;
		cout << "Error detected in run time. This is error number " << globalErrorCount << endl; 
		return; 
	}
	else
		cout << "Setup error detected.";
	char keyboardCommand;
	cout << " Please press E to exit program" << endl; 
	while (1)
	{
		while (!(keyboardCommand = CheckKeyboard())) {}; // Wait for input
		if (keyboardCommand == 'E')
			exit(1);
		else
			keyboardCommand = 0;
	}
}