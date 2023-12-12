#include "connection_bridge.hpp"

ConnectionBridge::ConnectionBridge(int clientSocket) {
    this->clientSocket = clientSocket;
    this->running = true;
    std::cout << "[ConnectionBridge] Object created." << std::endl;
}

ConnectionBridge::~ConnectionBridge() {
    this->running = false;
    
    close(clientSocket);
    close(remoteSocket);
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
    while ((bytesRead = recv(this->clientSocket, buffer, sizeof(buffer), 0)) > 0 && running == true) { // error
        std::cout << "[ConnectionBridge] Received data from client: " << std::string(buffer, bytesRead) << std::endl;
        if(this->isConnectMethod(buffer, bytesRead)){
            std::string host = this->parseHost(buffer, bytesRead);
            std::cout << "[ConnectionBridge] Parsed host: " << host << std::endl;
        }
    }
    std::cout << "[ConnectionBridge] Listening client thread stopped." << std::endl;
    this->running = false;
}

bool ConnectionBridge::isConnectMethod(char* buffer, int buffersize) {
    std::string method = std::string(buffer, 7);
    if(method == "CONNECT") {
        return true;
    }
    return false;
}

std::string ConnectionBridge::parseHost(char* buffer, int buffersize) {
    if (buffer == nullptr || buffersize <= 0) {
        return ""; 
    }

    const char* start = std::strstr(buffer, "Host:");
    if (start == nullptr) {
        return ""; 
    }

    start += 5; 
    const char* end = std::strchr(start, '\n'); 
    if (end == nullptr) {
        end = buffer + buffersize; 
    }
    std::string host(start, end - start);

    return host;
}
