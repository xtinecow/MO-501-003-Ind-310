#include "ComputerProgram.h"
// This file will include all keyboard commands that the user can enter to affect the program

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