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
    bool isConnectMethod(char* buffer, int size);
    std::string parseHost(char* buffer, int size);


    int clientSocket = -1;
    int remoteSocket = -1;
    std::atomic<bool> running;
    std::thread clientSocketListenThread;
};

#endif // _CONNECTION_BRIDGE_HPP
