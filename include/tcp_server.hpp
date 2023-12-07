#ifndef _TCP_SERVER_HPP
#define _TCP_SERVER_HPP

#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <vector>
#include <mutex>
#include "connection_bridge.hpp"


class TCPServer
{
public:
    TCPServer(int port);
    ~TCPServer();
    int run();
private:
    void checkBridges();

    int port;
    bool running;
    int serverSocket;
    std::vector<ConnectionBridge*>* activeBridges;
    std::mutex checkBridgeMutex;
    std::mutex createBridgeMutex;
};

#endif // _TCP_SERVER_HPP
