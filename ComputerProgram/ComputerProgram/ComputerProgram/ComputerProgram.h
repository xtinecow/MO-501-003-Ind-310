// Main header file for the project. Add all libraries in here. 

#include<stdio.h.>
#include<string.h>
#include<windows.h>
#include <iostream>

// Serial library added from blog post
// https://www.codeguru.com/cpp/i-n/network/serialcommunications/article.php/c2503/CSerial--A-C-Class-for-Serial-Communications.htm
// Instructions on this blog post were followed to set up serial communication
// NOTE: The Serial.h file had to be editted to include the necessary headers for the class.
// The cpp file was also edited to cast the char * elements of szPort and szComParams to their desired variables 
// This seems to compile now. The original files were kept under ComputerProgram/Serial Class for future reference
#include "Serial.h"

// Function prototypes

// Commands.c
char CheckKeyboard(void);

