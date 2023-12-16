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
    char buffer[1500];
    int bytesRead;
    while ((bytesRead = recv(this->clientSocket, buffer, sizeof(buffer), 0)) > 0 && running == true) { // error
        //std::cout << "[ConnectionBridge] Received data from client: " << std::string(buffer, bytesRead) << std::endl;
        if(this->isConnectMethod(buffer, bytesRead)){
            std::string host = this->parseHost(buffer, bytesRead);
            std::cout << "[ConnectionBridge] Parsed host: <" << host << ">" << std::endl;
            this->createTcpConnection(host);
            char *message = "HTTP/1.1 200 OK\n\n\0";
            this->writeToClientSocket(message, strlen(message));
        }
        else {
            //std::cout << "[ConnectionBridge] Writing data from client to remote: " << std::string(buffer, bytesRead) << std::endl;
            this->writeToRemoteSocket(buffer, bytesRead);
        }
        
    }
    std::cout << "[ConnectionBridge] Listening client thread stopped." << std::endl;
    this->running = false;
}

void ConnectionBridge::listenRemoteSocket() {
    char buffer[1500];
    int bytesRead;
    while ((bytesRead = recv(this->remoteSocket, buffer, sizeof(buffer), 0)) > 0 && running == true) { // error
        this->writeToClientSocket(buffer, bytesRead);
    }
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

void ConnectionBridge::createTcpConnection(std::string& host) {
    size_t colonPos = host.find(':');
    if (colonPos == std::string::npos) {
        std::cerr << "[ConnectionBridge] Error: Expected a valid host:port format." << std::endl;
        return;
    }
    std::string hostname = host.substr(0, colonPos);
    std::string portStr = host.substr(colonPos + 1);
    int port;
    std::istringstream(portStr) >> port;
    this->remoteSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->remoteSocket == -1) {
        perror("[ConnectionBridge] Socket creation failed");
        return;
    }
    struct hostent *server = gethostbyname(hostname.c_str());
    if (server == nullptr) {
        perror("[ConnectionBridge] Host resolution failed");
        close(this->remoteSocket);
        return;
    }
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    memcpy(&serverAddr.sin_addr.s_addr, server->h_addr, server->h_length);
    serverAddr.sin_port = htons(port);
    if (connect(this->remoteSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("[ConnectionBridge] Connection failed");
        close(this->remoteSocket);
        return;
    }
    std::cout << "[ConnectionBridge] Connection successful to host: " << host << std::endl;
    std::thread thread = std::thread(&ConnectionBridge::listenRemoteSocket, this);
    thread.detach();
}

void ConnectionBridge::writeToClientSocket(char* data, size_t dataSize) {

    if (this->clientSocket == -1) {
        std::cerr << "[ConnectionBridges] Client socket not set." << std::endl;
        return;
    }
    std::cout << "[ConnectionBridges] Write data to client socket." << std::endl;
    ssize_t bytes_sent = send(this->clientSocket, data, dataSize, 0);
    //std::cout << "[ConnectionBridge] Data send to client: " << data  << "size: " << dataSize << std::endl;
    if (bytes_sent == -1) {
        std::cerr << "[ConnectionBridges] Error writing data to client socket." << std::endl;
        return;
    }
}

void ConnectionBridge::writeToRemoteSocket(char* data, size_t dataSize) {
    if (this->remoteSocket == -1) {
        std::cerr << "[ConnectionBridges] Remote socket not set." << std::endl;
        return;
    }
    ssize_t bytesSent = send(this->remoteSocket, data, dataSize, 0);
    if (bytesSent == -1) {
        std::cerr << "[ConnectionBridges] Error writing data to remote socket." << std::endl;
        return;
    }
    std::cout <<  "[ConnectionBridges] Write data to remote socket." << std::endl;
}


