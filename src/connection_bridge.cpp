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
            std::cout << "[ConnectionBridge] Parsed host: <" << host << ">" << std::endl;
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
    std::string bufferString(buffer, buffersize);
    size_t newlinePos = bufferString.find('\n');
    std::string firstLine = "";
    if (newlinePos != std::string::npos) {
        firstLine = bufferString.substr(0, newlinePos);
    }
    std::string result = "";
    bool addFlag = false;
    for (size_t i = 0; i < firstLine.size(); i++) {
        if(firstLine.at(i) == ' '){
            addFlag = !addFlag;
            continue;
        }
        if(addFlag) {
            result += firstLine.at(i);
        }
    }
    return result;
}
