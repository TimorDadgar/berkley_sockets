#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include "client.hpp"

using namespace std;

#define DEFAULT_PORT "45000"

int main() {
    // Initialize Winsock
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return 1;
    }

    // Create a socket for sending data
    SOCKET sendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sendSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Set up the server address
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
    serverAddr.sin_port = htons(stoi(DEFAULT_PORT));

    // Send data to the server
    const char* data = "Hello from client";
    int bytesSent = sendto(sendSocket, data, strlen(data), 0, (const sockaddr*)&serverAddr, sizeof(serverAddr));
    if (bytesSent == SOCKET_ERROR) {
        std::cerr << "sendto failed: " << WSAGetLastError() << std::endl;
        closesocket(sendSocket);
        WSACleanup();
        return 1;
    }

    // Clean up
    closesocket(sendSocket);
    WSACleanup();

    return 0;
}
