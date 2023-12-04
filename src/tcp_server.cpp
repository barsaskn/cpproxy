#include "TCPServer.hpp"

int TCPServer::run() {
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
    }
}