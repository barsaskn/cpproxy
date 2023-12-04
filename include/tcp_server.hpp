#ifndef _TCP_SERVER_HPP
#define _TCP_SERVER_HPP

#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>


class TCPServer
{
public:
    TCPServer(int port) : port(port), running(false) {}
    ~TCPServer();
    int run();
private:
    int port;
    bool running;
    int serverSocket;
};

#endif // _TCP_SERVER_HPP
