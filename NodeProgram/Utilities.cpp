#include"NodeProgram.h"
using namespace std;

void ParseFNResponse(char *response, int size)
{
    int node,macByte, numNodes, macIndex, rssiIndex;

    numNodes = size/FN_RESPONSE_SIZE;

    for(node=0; node<numNodes; node++)
    {
        for(macByte=0; macByte<8; macByte++)
        {
            macIndex = node*FN_RESPONSE_SIZE+5+macByte; // Offset of 5 for FFFE\r
            NodeTable[node].MAC[macByte] = response[macIndex];
        }
        for(macByte=0; macByte<8; macByte++)
        {
            macIndex = node*FN_RESPONSE_SIZE+14+macByte; // Skip carriage return in middle of MAC
            NodeTable[node].MAC[8+macByte] = response[macIndex];
        }
        rssiIndex = 46;
        NodeTable[node].RSSI = ConvertHexByteToInt(&response[46]);
    }
}

// Take the 2 chars of HEX value and convert it to an int
int ConvertHexByteToInt (char* pointer)
{
    int i, dig;
    int sum = 0;

    for(i=0; i<2; i++)
    {
        switch(pointer[0])
        {
            case '0': dig=0;
                    break;
            case '1': dig=1;
                    break;
            case '2': dig=2;
                    break;
            case '3': dig=3;
                    break;
            case '4': dig=4;
                    break;
            case '5': dig=5;
                    break;
            case '6': dig=6;
                    break;
            case '7': dig=7;
                    break;
            case '8': dig=8;
                    break;
            case '9': dig=9;
                    break;
            case 'A': dig=10;
                    break;
            case 'B': dig=11;
                    break;
            case 'C': dig=12;
                    break;
            case 'D': dig=13;
                    break;
            case 'E': dig=14;
                    break;
            case 'F': dig=15;
                    break;

            default:
                cout << "Invalid character " << pointer[i] << " entered" << endl;
                break;

            }
            if(!i) // First digit
                sum += 16*dig;
            else
                sum += dig;
    }
    return sum;
}

void DisplayNodeTable(void)
{
    int i, node;

    for(node=0; node<MAX_NUM_NODES; node++)
    {
        cout << "Node number: " << node << endl;
        cout << "MAC: ";
        for(i=0; i<8; i++)
            cout << NodeTable[node].MAC[2*i] << NodeTable[node].MAC[2*i+1] << ":";
        cout << endl;
        cout << "RSSI: " << NodeTable[node].RSSI << endl;
    }
}
