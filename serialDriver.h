#ifndef __SERIALDRIVER_H__
#define __SERIALDRIVER_H__

#include <strings.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <stdexcept>
#include <exception>

constexpr char USB0[] = "/dev/ttyUSB0";
constexpr char USB1[] = "/dev/ttyUSB1";

class serialDriver
{
    public:
        serialDriver(const serialDriver&) = delete;
        serialDriver(serialDriver&&) = delete;
        serialDriver(char _inputDevice[]) noexcept(false);
        serialDriver(void) noexcept(false);
        ~serialDriver(void) noexcept(false);
        serialDriver& operator = (const serialDriver&) = delete;
        serialDriver& operator = (serialDriver&&) = delete;
        void sendData(char* _input) noexcept(false);
        void receiveData(char* _ouput) noexcept(false);
    
    private:
        int m_intFile_descriptor;
        struct termios m_tty;
        void openPort(const char _inputDevice[]) noexcept(false);
};

#endif