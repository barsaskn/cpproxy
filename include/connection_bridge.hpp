#ifndef _CONNECTION_BRIDGE_HPP
#define _CONNECTION_BRIDGE_HPP

#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <atomic>
#include <cstring>

class ConnectionBridge
{
public:
    ConnectionBridge(int clientSocket);
    ~ConnectionBridge();
    int run();
    bool isRunning();

private:
    void listenClientSocket();
    void parseMessage();
    int clientSocket;
    int remoteSocket;
    std::atomic<bool> running;
    std::thread clientSocketListenThread;
};

#endif // _CONNECTION_BRIDGE_HPP
