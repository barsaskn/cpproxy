#include "connection_bridge.hpp"

ConnectionBridge::ConnectionBridge(int clientSocket) {
    this->clientSocket = clientSocket;
    this->running = true;
    std::cout << "[ConnectionBridge] Object created." << std::endl;
}

ConnectionBridge::~ConnectionBridge() {
    std::cout << "[ConnectionBridge] Object deleted." << std::endl;
}

int ConnectionBridge::run(){
    this->clientSocketListenThread = std::thread(&ConnectionBridge::listenClientSocket, this);
    this->clientSocketListenThread.detach();
    std::cout << "[ConnectionBridge] Client listening thread started." << std::endl;
    return 0;
}

bool ConnectionBridge::isRunning() {
    return this->running;
}

void ConnectionBridge::listenClientSocket() {
    char buffer[1024];
    int bytesRead;
    while ((bytesRead = recv(this->clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        std::cout << "[ConnectionBridge] Received data from client: " << std::string(buffer, bytesRead) << std::endl;
    }
    std::cout << "[ConnectionBridge] Listening of the socket ended." << std::endl;
    this->running = false;
    close(clientSocket);
}
