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
    std::atomic<bool> running;
    int serverSocket;
    std::vector<ConnectionBridge*>* activeBridges;
    std::mutex vectorMutex;
    std::thread checkBridgesThread;
};

#endif // _TCP_SERVER_HPP
