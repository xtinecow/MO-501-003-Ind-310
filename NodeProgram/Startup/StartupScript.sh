#!/bin/bash

echo capstone | sudo -S interceptty -s 'ispeed 9600 ospeed 9600' /dev/ttyUSB0 /dev/interceptPtty & 
echo capstone | sudo -S bash StartNode.sh
