#include "tcp_server.hpp"

int main() {
    TCPServer* tcpServer = new TCPServer(8080);
    tcpServer->run();
    std::cout << "server";
    return 0;
}