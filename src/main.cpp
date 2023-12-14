#include <cstdlib>
#include <csignal>
#include "tcp_server.hpp"

TCPServer* tcpServer;

void exitFunction(int signum) {
    delete tcpServer;
    std::cout << "[MAIN] App is closing..." << std::endl;
}

int main() {
    
    std::cout << "[MAIN] App is starting..." << std::endl;
    signal(SIGINT, exitFunction);
    tcpServer = new TCPServer(8080);
    tcpServer->run();
    return 0;
}