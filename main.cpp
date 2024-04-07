#include "serialDriver.h"

int main()
{
    char msg[] = { 'H', 'e', 'l', 'l', 'o', '\r' };
    char recvmsg[12];
    memset(recvmsg, 0, sizeof(recvmsg));
    //serialDriver serSend("/dev/ttyUSB0");
    serialDriver serRecv("/dev/ttyUSB0");
    //serSend.sendData(msg);
    for (int i=0 ; i< 5000; i++)
    {
        usleep(500);
        serRecv.receiveData(recvmsg);
    }
    // int serial_port = open("/dev/ttyUSB0", O_RDWR);
    printf("Received data %s\n",recvmsg);
    // printf("Serial port num %d \n" , serial_port);
    // close(serial_port);

    return 0;

}