#include <cstdlib>
#include "tcp_server.hpp"

TCPServer* tcpServer;

void exitFunction(int signum) {
    delete tcpServer;
    std::cout << "[MAIN] App is closing" << std::endl;
}

int main() {
    //atexit(exitFunction);
    signal(SIGINT, exitFunction);
    tcpServer = new TCPServer(8080);
    tcpServer->run();
    return 0;
}