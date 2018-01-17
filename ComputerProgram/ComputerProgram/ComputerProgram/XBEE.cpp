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
	cout << "done." << endl;
	Sleep(1000); // wait for the required guard time (default of 1000 ms)

	// Poll for 'OK' Response 
	cout << "Waiting for OK Response... ";
	timeout = clock() + CLOCKS_PER_SEC; // Give it 1 sec to respond
	nBytesRead = 0;
	while (clock() < timeout)
	{
		nBytesRead += serial.ReadData(response, 20);
		if (nBytesRead == 3)
		{
			cout << response[0] << response[1] << endl;
			break; 
		}
		Sleep(100); // Only pull every 100 ms
	}
	if (!nBytesRead)
	{
		cout << "Error reading from serial port"<< endl;
		WaitForExit();
	}

	// Set command mode timeout to max value (0x1770)
	command[0] = 'A';
	command[1] = 'T';
	command[2] = 'C';
	command[3] = 'T'; 
	command[4] = '0';
	command[5] = 'x';
	command[6] = '1';
	command[7] = '7';
	command[8] = '7';
	command[9] = '0';
	command[10] = 13; // Append carriage return

	cout << "Setting command mode timeout to max value... ";
	nBytesSent = 0;
	nBytesSent = serial.SendData(command, 11);
	if (!nBytesSent)
	{
		cout << "Error writing to serial port" << endl;
		WaitForExit();
	}
	cout << "done" << endl; 

	// Poll for 'OK' Response 
	cout << "Waiting for OK Response... ";
	timeout = clock() + CLOCKS_PER_SEC; // Give it 1 sec to respond
	nBytesRead = 0;
	while (clock() < timeout)
	{
		nBytesRead += serial.ReadData(response, 20);
		if (nBytesRead == 3)
		{
			cout << response[0] << response[1] << endl;
			break;
		}
		Sleep(100); // Only pull every 100 ms
	}
	if (!nBytesRead)
	{
		cout << "Error reading from serial port" << endl;
		WaitForExit();
	}

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
	cout << "done." << endl;


	// Poll for 'OK' Response 
	cout << "Waiting for OK Response... ";
	timeout = clock() + CLOCKS_PER_SEC; // Give it 1 sec to respond
	nBytesRead = 0;
	while (clock() < timeout)
	{
		nBytesRead = serial.ReadData(response, 20);
		if (nBytesRead == 3)
		{
			cout << response[0] << response[1] << endl;
			break;
		}
		Sleep(100); // Only pull every 100 ms
	}
	if (!nBytesRead)
	{
		cout << "Error reading from serial port" << endl;
		WaitForExit();
	}



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

void CloseSerialPort()
{
	if (serial.Close())
		cout << "Port successfully closed" << endl;
	else
	{
		cout << "Failed to close port "<< endl;
		WaitForExit();
	}
}