#include "tcp_server.hpp"

TCPServer::TCPServer(int port) {
    this->running = true;
    this->port = port;
    this->activeBridges = new std::vector<ConnectionBridge*>();
}

TCPServer::~TCPServer() {
    this->running = false;
    if(checkBridgesThread.joinable()) {
        checkBridgesThread.join();
    }
    shutdown(this->serverSocket, SHUT_RDWR);
    std::unique_lock<std::mutex> lock(this->vectorMutex);
    for (auto it = this->activeBridges->begin(); it != this->activeBridges->end();) {
        delete (*it);
        it = this->activeBridges->erase(it);
        continue;
    }
    if (close(serverSocket) == 0) {
        std::cout << "[TCPServer] Socket closed successfully." << std::endl;
    }
    std::cout << "[TCPServer] Object deleted." << std::endl;
}

int TCPServer::run() {
    checkBridgesThread = std::thread(&TCPServer::checkBridges, this);
    
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "[TCPServer] Error creating socket." << std::endl;
        return -1;
    }
    std::cout << "[TCPServer] Socket Created." << std::endl;

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);
    int enable = 1;

    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        std::cerr << "setsockopt(SO_REUSEADDR) failed" << std::endl;
        close(serverSocket);
        return -1;
    }

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "[TCPServer] Error binding socket to port." << std::endl;
        close(serverSocket);
        return -1;
    }
    std::cout << "[TCPServer] Socket Bound." << std::endl;

    if (listen(serverSocket, 10) == -1) {
        std::cerr << "[TCPServer] Error listening on socket." << std::endl;
        close(serverSocket);
        return -1;
    }

    std::cout << "[TCPServer] Socket listening for new clients." << std::endl;
    while (this->running == true) {
        int clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket == -1) {
            std::cerr << "[TCPServer] Error accepting client connection." << std::endl;
            break;
        }
        std::cout << "[TCPServer] New client: " << clientSocket << std::endl;
        std::unique_lock<std::mutex> lock(this->vectorMutex);
        ConnectionBridge* connectionBridge = new ConnectionBridge(clientSocket); 
        this->activeBridges->push_back(connectionBridge);
        connectionBridge->run();
    }

    std::cout << "[TCPServer] Socket listening stopped." << std::endl;
    return 0;
}

void TCPServer::checkBridges() {
    std::cout << "[TCPServer] Check Bridge thread started." << std::endl;
    while (this->running == true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::unique_lock<std::mutex> lock(this->vectorMutex);
        for (auto it = this->activeBridges->begin(); it != this->activeBridges->end(); ) {
            if (!(*it)->isRunning()) {
                delete *it;
                it = this->activeBridges->erase(it);
                continue;
            } 
            ++it;
        }
        lock.unlock();
    }
    std::cout << "[TCPServer] Check Bridge thread stopped." << std::endl;
}