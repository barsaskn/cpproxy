#ifndef _CONNECTION_BRIDGE_HPP
#define _CONNECTION_BRIDGE_HPP

#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <atomic>
#include <cstring>
#include <netdb.h>
#include <sstream>

class ConnectionBridge
{
public:
    ConnectionBridge(int clientSocket);
    ~ConnectionBridge();
    int run();
    bool isRunning();

private:
    void listenClientSocket();
    void listenRemoteSocket();
    bool isConnectMethod(char* buffer, int size);
    std::string parseHost(char* buffer, int size);
    void createTcpConnection(std::string& host);
    void writeToClientSocket(char *data, size_t dataSize);
    void writeToRemoteSocket(char *data, size_t dataSize);

    int clientSocket = -1;
    int remoteSocket = -1;
    std::atomic<bool> running;
    std::thread clientSocketListenThread;
    std::mutex remoteSocketMutex;
    std::mutex clientSocketMutex;
};

#endif // _CONNECTION_BRIDGE_HPP
