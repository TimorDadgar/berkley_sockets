#include <iostream>
#include <chrono>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <memory>
using namespace std;

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "server.hpp"


SocketAddress::SocketAddress(uint32_t inAddress, uint16_t inPort)
{
    GetAsSockAddrIn()->sin_family = AF_INET;
    GetAsSockAddrIn()->sin_addr.S_un.S_addr = htonl(inAddress);
    GetAsSockAddrIn()->sin_port = htons(inPort);
}


int UDPSocket::Bind(const SocketAddress& inBindAddress)
{
    int err = bind(mSocket, &inBindAddress.mSockAddr, inBindAddress.GetSize());

    if(err != 0)
    {
        // Should have class to handle error reporting
        // SocketUtil::ReportError(L"UDPSocket::Bind");
        // return SocketUtil::GetLastError();
        return -1;
    }
    return NO_ERROR;
}

int UDPSocket::SendTo(const void* inData, int inLen,
                        const SocketAddress& inTo)
{
    int byteSentCount = sendto( mSocket, static_cast<const char*>( inData), inLen, 0, &inTo.mSockAddr, inTo.GetSize());

    if(byteSentCount >= 0)
    {
        return byteSentCount;
    }
    else
    {
        //return error as negative number
        // SocketUtil::ReportError(L"UDPSocket::SendTo");
        return -1;
    }
}

int UDPSocket::ReceiveFrom(void* inBuffer, int inMaxLength, SocketAddress& outFrom)
{
    int fromLength = outFrom.GetSize();
    int readByteCount = recvfrom(mSocket, static_cast<char*>(inBuffer), inMaxLength,
     0, &outFrom.mSockAddr, &fromLength);

    if(readByteCount >= 0)
    {
        return readByteCount;
    }
    else
    {
        // SocketUtil::ReportError(L"UDPSocket::ReceiveFrom");
        // return -SocketUtil::GetLastError();
        return -1;
    }
}

UDPSocket::~UDPSocket()
{
    closesocket(mSocket);
}

int Server::berkley_socket()
{
    // Initialize Winsock
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        printf("WSAStartup failed: %d\n", result);
        return 1;
    }

    // Create a socket
    SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (serverSocket == INVALID_SOCKET) {
        printf("Error creating socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    sockaddr_in inServerAddr;
    memset(inServerAddr.sin_zero, 0, sizeof(inServerAddr.sin_zero));
    inServerAddr.sin_family = AF_INET;
    inServerAddr.sin_port = htons(45000);
    inServerAddr.sin_addr.S_un.S_un_b.s_b1 = 65;
    inServerAddr.sin_addr.S_un.S_un_b.s_b2 = 254;
    inServerAddr.sin_addr.S_un.S_un_b.s_b3 = 248;
    inServerAddr.sin_addr.S_un.S_un_b.s_b4 = 180;
    inServerAddr.sin_addr.s_addr = INADDR_ANY;

    LPCSTR ipv4_address_custom = "127.0.0.1";
    // Does the work of translating human readable string into socket address/binary IP address
    InetPton(AF_INET, ipv4_address_custom, &inServerAddr.sin_addr);
    // // InetPton does not do a DNS lookup, therefore use this then
    // int getaddrinfo(const char *hostname, const char *servname, const addrinfo *hints, addrinfo **res);

    // UDP Socket Class Wrapper for Type Safety and security abstraction
    this->UDPSocketRef = std::make_shared<UDPSocket>(serverSocket);
    // The octets are: 127 (0xC0), 0, 0, 1, using bitwise operators
    uint32_t ipAddress = (127 << 24) | (0 << 16) | (0 << 8) | 1;
    // Example port number: 8080
    uint16_t port = 45000;
    SocketAddress ServerAddr(ipAddress, port);

    UDPSocketRef->Bind(ServerAddr);
    // bind(serverSocket, (struct sockaddr*)&inServerAddr, sizeof(inServerAddr));

    // Send Data to client
    // MTU for Ethernet is 1500 bytes,
    const char* data = "W"; 

    outBallState->mPosX = 11;

    SocketAddress ClientInAddr(ipAddress, port);
    // a good rule of thumb is to avoid sending datagrams with data larger than 1300 bytes.
    UDPSocketRef->SendTo(reinterpret_cast<const char*>(outBallState.get()), sizeof(BallState), ClientInAddr);
    // UDPSocketRef->SendTo(data, strlen(data), ClientInAddr);
    return 0;
}


int Server::client_input(){
    uint32_t ipAddress = (127 << 24) | (0 << 16) | (0 << 8) | 1;
    char buffer[1300];
    sockaddr_in fromAddr;
    int fromlen = sizeof(fromAddr);
    SocketAddress fromSocketAddr(ipAddress, 0);
    
    // using Type Safe UDP class instead of direct call
    int bytesReceived = UDPSocketRef->ReceiveFrom(buffer, sizeof(buffer), fromSocketAddr);
    // int bytesReceived = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (sockaddr*)&fromAddr, &fromlen);
    if (bytesReceived == -1)
    {
        printf("recvfrom failed: %d\n", WSAGetLastError());
        return 1;
    }; 
    std::cout << "Bytes received: " << bytesReceived << std::endl;
    // Print the received data up to the number of bytes received
    std::string client_input_data(buffer, bytesReceived);
    std::cout << client_input_data << std::endl;
    
    return 0;
}

int Server::clean_up(){
    // int bytesSent = sendto(serverSocket, data, strlen(data), 0, (const sockaddr*)&inServerAddr, sizeof(inServerAddr));
    if(shutdown(UDPSocketRef->mSocket, SD_SEND)){
        printf("Error shutting down socket: %d\n", WSAGetLastError());
    }

    if(closesocket(UDPSocketRef->mSocket)){
        printf("Error shutting down socket: %d\n", WSAGetLastError());
    }

    // Shutdown windows socket interface
    int result = WSACleanup();
    if (result != 0) {
        printf("WSACleanup failed: %d\n", WSAGetLastError());
        return 1;
    }
    return 0;
}


// Window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Paddle dimensions
const int PADDLE_WIDTH = 20;
const int PADDLE_HEIGHT = 100;

// Ball dimensions and speed
const int BALL_SIZE = 15;
const int BALL_SPEED = 5;


int Server::start_pong_game(){
    
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow("Pong Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Paddle positions
    int leftPaddleY = (WINDOW_HEIGHT - PADDLE_HEIGHT) / 2;
    int rightPaddleY = (WINDOW_HEIGHT - PADDLE_HEIGHT) / 2;

    // Ball position and velocity
    int ballX = WINDOW_WIDTH / 2;
    int ballY = WINDOW_HEIGHT / 2;
    int ballVelX = BALL_SPEED;
    int ballVelY = BALL_SPEED;

    // Game loop
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        // Event handling
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Move paddles with arrow keys
        const Uint8* keystates = SDL_GetKeyboardState(NULL);
        if (keystates[SDL_SCANCODE_UP]) {
            rightPaddleY -= 5;
        }
        if (keystates[SDL_SCANCODE_DOWN]) {
            rightPaddleY += 5;
        }
        if (keystates[SDL_SCANCODE_W]) {
            leftPaddleY -= 5;
        }
        if (keystates[SDL_SCANCODE_S]) {
            leftPaddleY += 5;
        }

        // Keep paddles within window bounds
        if (leftPaddleY < 0) {
            leftPaddleY = 0;
        }
        if (leftPaddleY > WINDOW_HEIGHT - PADDLE_HEIGHT) {
            leftPaddleY = WINDOW_HEIGHT - PADDLE_HEIGHT;
        }
        if (rightPaddleY < 0) {
            rightPaddleY = 0;
        }
        if (rightPaddleY > WINDOW_HEIGHT - PADDLE_HEIGHT) {
            rightPaddleY = WINDOW_HEIGHT - PADDLE_HEIGHT;
        }

        // Move the ball
        ballX += ballVelX;
        ballY += ballVelY;

        // Ball collision with top and bottom walls
        if (ballY <= 0 || ballY >= WINDOW_HEIGHT - BALL_SIZE) {
            ballVelY = -ballVelY;
        }

        // Ball collision with paddles
        if ((ballX <= PADDLE_WIDTH && ballY + BALL_SIZE >= leftPaddleY && ballY <= leftPaddleY + PADDLE_HEIGHT) ||
            (ballX >= WINDOW_WIDTH - PADDLE_WIDTH - BALL_SIZE && ballY + BALL_SIZE >= rightPaddleY && ballY <= rightPaddleY + PADDLE_HEIGHT)) {
            ballVelX = -ballVelX;
        }

        // Ball out of bounds
        if (ballX <= 0 || ballX >= WINDOW_WIDTH) {
            ballX = WINDOW_WIDTH / 2;
            ballY = WINDOW_HEIGHT / 2;
        }

        // Draw paddles
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect leftPaddle = { 0, leftPaddleY, PADDLE_WIDTH, PADDLE_HEIGHT };
        SDL_Rect rightPaddle = { WINDOW_WIDTH - PADDLE_WIDTH, rightPaddleY, PADDLE_WIDTH, PADDLE_HEIGHT };
        SDL_RenderFillRect(renderer, &leftPaddle);
        SDL_RenderFillRect(renderer, &rightPaddle);

        // Draw the ball
        SDL_Rect ball = { ballX, ballY, BALL_SIZE, BALL_SIZE };
        SDL_RenderFillRect(renderer, &ball);

        // Update the screen
        SDL_RenderPresent(renderer);

        // Cap frame rate
        SDL_Delay(10);
    }

    // Clean up and quit SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}