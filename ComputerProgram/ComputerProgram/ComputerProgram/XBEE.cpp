// This file will handle all communication with the XBEE module 

#include"ComputerProgram.h"
using namespace std; 

CSerial serial;


void OpenSerialPort(int portNum)
{
	if (serial.Open(portNum, XBEE_BAUDRATE))
		cout << "Port successfully opened" << endl;
	else
	{
		cout << "Failed to open port " << portNum << endl;
		WaitForExit();
	}
}

void SetATCommandMode(void)
{
	int nBytesSent, nBytesRead, i; 
	clock_t timeout;
	char response[20];
	char command[20]; 
	// Put device in command mode by sending 3 consecutive '+'commands
	cout << "Putting module in command mode... ";
	Sleep(1000); // wait for the required guard time (default of 1000 ms)
	for (i = 1; i <= 3; i++)
	{
		nBytesSent = 0;
		nBytesSent = serial.SendData("+", 1);
		if (!nBytesSent)
		{
			cout << "Error writing to serial port" << endl;
			WaitForExit(); 
		}
		Sleep(50); 
	}
	cout << "done.	"; 
	Sleep(1000); // wait for the required guard time (default of 1000 ms)

	CheckForOKResponse(); 

	//// Set command mode timeout to max value (0x1770) - No longer used
	//command[0] = 'A';
	//command[1] = 'T';
	//command[2] = 'C';
	//command[3] = 'T'; 
	//command[4] = '0';
	//command[5] = 'x';
	//command[6] = '1';
	//command[7] = '7';
	//command[8] = '7';
	//command[9] = '0';
	//command[10] = 13; // Append carriage return

	//cout << "Setting command mode timeout to max value... ";
	//nBytesSent = 0;
	//nBytesSent = serial.SendData(command, 11);
	//if (!nBytesSent)
	//{
	//	cout << "Error writing to serial port" << endl;
	//	WaitForExit();
	//}
	//cout << "done" << endl; 

	//CheckForOKResponse(); 

	//ApplyChangeCommand(); 



}

void ReadFirmwareVersion(void)
{
	int nBytesSent, nBytesRead;
	char command[20];
	char response[20];
	clock_t timeout;

	command[0] = 'A'; 
	command[1] = 'T';
	command[2] = 'V';
	command[3] = 'R'; 
	command[4] = 13; // Add carriage return 
	cout << "Reading firmware version... ";
	nBytesSent = 0;
	nBytesSent = serial.SendData(command, 5);
	if (!nBytesSent)
	{
		cout << "Error writing to serial port" << endl;
		WaitForExit();
	}
	timeout = clock() + CLOCKS_PER_SEC; // Give it 1 sec to respond
	nBytesRead = 0; 
	while (clock() < timeout)
	{
		nBytesRead += serial.ReadData(response, 20);
		if (nBytesRead == 5)
		{
			cout << "0x" << response[0] << response[1] << response[2] << response[3] << endl; 
			return; 
		}
		Sleep(100); // Only pull every 100 ms
	}
	cout << "Error reading from serial port" << endl;
	WaitForExit();

}

// Put the module in API mode 1. This means only API frames will be accepted (no Escape characters enabled) 
void SetAPIMode(void)
{
	cout << "Setting API Mode... "; 
	char command[12]; 
	char response[12]; 
	int nBytesSent, nBytesRead; 
	clock_t timeout; 
	command[0] = 'A';
	command[1] = 'T';
	command[2] = 'A';
	command[3] = 'P';
	command[4] = '1';
	command[5] = 13; // Append carriage return
	nBytesSent = 0;
	nBytesSent = serial.SendData(command, 6);
	if (!nBytesSent)
	{
		cout << "Error writing to serial port" << endl;
		WaitForExit();
	}
	cout << "done.	";


	CheckForOKResponse();
	ApplyChangeCommand(); 

}

// Moving this to its own function because it gets called a lot
void CheckForOKResponse(void)
{ 
	char response[4]; 
	int nBytesRead;
	clock_t timeout; 
	// Poll for 'OK' Response 
	cout << "Waiting for OK Response... ";
	timeout = clock() + CLOCKS_PER_SEC; // Give it 1 sec to respond
	nBytesRead = 0;
	while (clock() < timeout)
	{
		nBytesRead += serial.ReadData(response, 4);
		if (nBytesRead >= 3)
		{
			cout << response[0] << response[1] << endl;
			return;
		}
		Sleep(100); // Only pull every 100 ms
	}
	if (!nBytesRead)
	{
		cout << "Error checking for OK response" << endl;
		WaitForExit();
	}
}

// Moving this to its own function because it gets called a lot
void ApplyChangeCommand(void)
{
	char command[8]; 
	int nBytesSent; 

	// Apply change to module
	command[0] = 'A';
	command[1] = 'T';
	command[2] = 'A';
	command[3] = 'C';
	command[4] = 13; // Append carriage return
	cout << "Applying change.. ";
	nBytesSent = 0;
	nBytesSent = serial.SendData(command, 5);
	if (!nBytesSent)
	{
		cout << "Error writing to serial port" << endl;
		WaitForExit();
	}
	cout << "done.	";

	CheckForOKResponse(); 
}

void ExitCommandMode (void)
{
	char command[8];
	int nBytesSent;

// Exit command mode
	command[0] = 'A';
	command[1] = 'T';
	command[2] = 'C';
	command[3] = 'N';
	command[4] = 13; // Append carriage return
	cout << "Applying change.. ";
	nBytesSent = 0;
	nBytesSent = serial.SendData(command, 5);
	if (!nBytesSent)
	{
		cout << "Error writing to serial port" << endl;
		WaitForExit();
	}
	cout << "done.	";

	CheckForOKResponse();
}

void CloseSerialPort(void)
{
	if (serial.Close())
		cout << "Port successfully closed" << endl;
	else
	{
		cout << "Failed to close port "<< endl;
		WaitForExit();
	}
}

void GetHostMAC(void)
{
	int nBytesSent, nBytesRead, i;
	char command[20];
	char response[20];
	clock_t timeout;

	cout << "Reading Host MAC... "; 

	// Only read lower half (upper half is always same)
	command[0] = 'A';
	command[1] = 'T';
	command[2] = 'S';
	command[3] = 'L';
	command[4] = 13; // Add carriage return 
	nBytesSent = 0;
	nBytesSent = serial.SendData(command, 5);
	if (!nBytesSent)
	{
		cout << "Error writing to serial port" << endl;
		WaitForExit();
	}
	timeout = clock() + CLOCKS_PER_SEC; // Give it 1 sec to respond
	nBytesRead = 0;
	HostMAC[0] = 0x00; 
	HostMAC[1] = 0x13;
	HostMAC[2] = 0xA2;
	HostMAC[3] = 0x00;
	while (clock() < timeout)
	{
		nBytesRead += serial.ReadData(&response[nBytesRead], 20);
		if (nBytesRead >= 9)
		{
			for (i = 0; i < 4; i++)
				HostMAC[4 + i] = (unsigned char)ConvertHexByteToInt(&response[2 * i]);
			cout << "MAC: ";
			// Print MAC address in hex format
			for (i = 0; i < 7; i++)
				cout << hex << uppercase << setw(2) << setfill('0') << (int)HostMAC[i] << ":";
			// Print last one and set format back to decimal
			cout << hex << uppercase << setw(2) << setfill('0') << (int)HostMAC[7] << dec << endl;
			// Set Node 0 to this MAC
			for (i = 0; i < 8; i++)
				NodeList[0].MAC[i] = HostMAC[i]; 

			return; 
		}
		Sleep(100); // Only pull every 100 ms
	}

	cout << "Error reading from serial port" << endl;
	WaitForExit();
}