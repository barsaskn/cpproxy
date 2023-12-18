#include <cstdlib>
#include <csignal>
#include "tcp_server.hpp"

TCPServer* tcpServer;

void handleSignal(int signum) {
    if(signum == SIGINT){
        delete tcpServer;
        std::cout << "[MAIN] App is closing..." << std::endl;
    }
    else if (signum == SIGPIPE){
        std::cout << "[MAIN] SIGPIPE signal emmited" << std::endl;
    }

}

int main() {
    std::cout << "[MAIN] App is starting..." << std::endl;
    signal(SIGINT, handleSignal);
    signal(SIGPIPE, handleSignal );
    tcpServer = new TCPServer(8080);
    tcpServer->run();
    return 0;
}