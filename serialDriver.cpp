#include "serialDriver.h"
#include <stdexcept>
#include <iostream>


serialDriver::serialDriver() noexcept(false)
{
    serialDriver(USB0);
}

serialDriver::serialDriver(char _inputDevice[]) noexcept(false)
{
    try{
        openPort(_inputDevice);
    }
    catch(const std::exception& e){
        std::cout << "Exception " << e.what() << std::endl;
        throw std::runtime_error(
            "Could not open the port you provided!"
        );
    }
}

void serialDriver::openPort(const char _inputDevice[]) noexcept(false)
{
    m_intFile_descriptor = open(_inputDevice, O_RDWR);
    if(m_intFile_descriptor<0)
    {
        throw std::runtime_error(std::string("Could not open port!"));
    }

    if(tcgetattr(m_intFile_descriptor, &m_tty) !=0)
    {
        std::cout << "Error " << errno << " from tcaetattr: " << strerror(errno) << std::endl;
        throw std::runtime_error(std::string("Tcgetattr error!"));
    }

    m_tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    m_tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    m_tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size 
    m_tty.c_cflag |= CS8; // 8 bits per byte (most common)
    m_tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    m_tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    m_tty.c_lflag &= ~ICANON;
    m_tty.c_lflag &= ~ECHO; // Disable echo
    m_tty.c_lflag &= ~ECHOE; // Disable erasure
    m_tty.c_lflag &= ~ECHONL; // Disable new-line echo
    m_tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    m_tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    m_tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

    m_tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    m_tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
    // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

    m_tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    m_tty.c_cc[VMIN] = 0;

    // Set in/out baud rate to be 9600
    cfsetispeed(&m_tty, B1152000);
    cfsetospeed(&m_tty, B1152000);

    if(tcsetattr(m_intFile_descriptor, TCSANOW, &m_tty) != 0) {
        std::cout << "Error " << errno << " from tcsetattr: " << strerror(errno) << std::endl;
        throw std::runtime_error(std::string("Tcsetattr error!"));
    }
}

void serialDriver::sendData(char * _input) noexcept(false)
{
    if(write(m_intFile_descriptor, _input, sizeof(_input))<0)
    {
        throw std::runtime_error(std::string("Could not write the correct number of data chars!"));
    }
}

void serialDriver::receiveData(char* _ouput) noexcept(false)
{
    while(read(m_intFile_descriptor, &_ouput, 255) >0)
    {
        printf("Got %s\n" , _ouput);
    }
    // if(read(m_intFile_descriptor, &_ouput, 255) < 0)
    // {
    //     throw std::runtime_error(std::string("Could not read the data stream!"));
    // }
    // printf("Got %s\n" , _ouput);
}

serialDriver::~serialDriver() noexcept(false)
{
    close(m_intFile_descriptor);
}