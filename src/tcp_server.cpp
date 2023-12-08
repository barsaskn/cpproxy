#include "tcp_server.hpp"

TCPServer::TCPServer(int port) {
    this->running = false;
    this->port = port;
    this->activeBridges = new std::vector<ConnectionBridge*>();
}

int TCPServer::run() {
    std::thread checkBridgesThread = std::thread(&TCPServer::checkBridges, this);
    checkBridgesThread.detach();
    
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "[TCPServer] Error creating socket." << std::endl;
        return -1;
    }
    std::cout << "[TCPServer] Socket Created." << std::endl;

    this->running = true;

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

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

    while (running) {
        int clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket == -1) {
            std::cerr << "[TCPServer] Error accepting client connection" << std::endl;
            continue;
        }
        std::cout << "[TCPServer] New client: " << clientSocket << std::endl;
        std::unique_lock<std::mutex> lock(this->vectorMutex);
        ConnectionBridge* connectionBridge = new ConnectionBridge(clientSocket); 
        this->activeBridges->push_back(connectionBridge);
        connectionBridge->run();
    }
    return 0;
}

void TCPServer::checkBridges() {
    std::cout << "[TCPServer] Check Bridge thread started." << std::endl;
    while (true) {
        //std::this_thread::sleep_for(std::chrono::seconds(1));//core dump if comment out
        std::unique_lock<std::mutex> lock(this->vectorMutex);
        for (auto it = this->activeBridges->begin(); it != this->activeBridges->end(); ) {
            if (!(*it)->isRunning()) {
                delete *it;
                it = this->activeBridges->erase(it);
                continue;
            } 
            ++it;
        }
    }
}