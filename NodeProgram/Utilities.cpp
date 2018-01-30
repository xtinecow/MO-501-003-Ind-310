#include"NodeProgram.h"
using namespace std;

void ParseFNResponse(char *response, int size)
{
    int node,macByte, numNodes, macIndex;

    while (response[0] != 'F')
    {
        response++; // Increment pointer when start of data is off
        size--; // Discarding first byte so size has to be decreased accordingly
        cout << "Aligning data..." << endl;
    }
        numNodes = size/FN_RESPONSE_SIZE;

    for(node=0; node<numNodes; node++)
    {
        for(macByte=0; macByte<4; macByte++)
        {
            macIndex = node*FN_RESPONSE_SIZE+5+2*macByte; // Offset of 5 for FFFE\r
            NodeTable[node].MAC[macByte] = (unsigned char)ConvertHexByteToInt(&response[macIndex]);
        }
        for(macByte=0; macByte<4; macByte++)
        {
            macIndex = node*FN_RESPONSE_SIZE+14+2*macByte; // Skip carriage return in middle of MAC
            NodeTable[node].MAC[4+macByte] = (unsigned char)ConvertHexByteToInt(&response[macIndex]);
        }
        NodeTable[node].RSSI = ConvertHexByteToInt(&response[node*FN_RESPONSE_SIZE+46]);
    }
}

// Take the 2 chars of HEX value and convert it to an int
int ConvertHexByteToInt (char* pointer)
{
    int i, dig;
    int sum = 0;

    for(i=0; i<2; i++)
    {
        switch(pointer[i])
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
                cout << endl << (int)pointer[i] << " entered at " << i<< endl;
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

    for(node=0; node<2/*MAX_NUM_NODES*/; node++) // Only display relevant nodes for now
    {
        cout << "Node number: " << node << endl;
        cout << "MAC: ";
        for(i=0; i<7; i++)
            cout << hex << uppercase << setw(2) << setfill('0') << (int)NodeTable[node].MAC[i] << ":";
        cout << hex << uppercase << setw(2) << setfill('0') << (int)NodeTable[node].MAC[7] << endl;
        cout << "RSSI: " << NodeTable[node].RSSI << endl;
    }
}
