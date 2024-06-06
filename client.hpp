#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <winsock2.h>
#include <ws2tcpip.h>
#include <memory>

#include "Ball/ball.h"

class SocketAddress
{
public:
    friend class UDPSocket;
    SocketAddress(uint32_t inAddress, uint16_t inPort);
    SocketAddress(const sockaddr& inSockAddr);
    size_t GetSize() const {return sizeof( sockaddr );}
private:
    sockaddr mSockAddr;
    sockaddr_in* GetAsSockAddrIn() {return reinterpret_cast<sockaddr_in*>( &mSockAddr );}
};
typedef std::shared_ptr<SocketAddress> SocketAddressPtr;

class UDPSocket
{
public:
    UDPSocket(SOCKET inSocket) : mSocket(inSocket) {};
    ~UDPSocket();
    int Bind(const SocketAddress& inToAddress);
    int SendTo(const void* inData, int inLen, const SocketAddress& inTo);
    int ReceiveFrom(void* inBuffer, int inLen, SocketAddress& outFrom);
    SOCKET mSocket;
private:
    friend class SocketUtil;
    // Have constructor like this if using SocketUtil
    // UDPSocket(SOCKET inSocket) : mSocket(inSocket) {};
};
typedef std::shared_ptr<UDPSocket> UDPSocketPtr;

class Client
{
public:
    Client(){};
    UDPSocketPtr UDPSocketRef;
    int start_pong_game();
    int berkley_socket();
    int clean_up();
    int server_input();
private: 
    char* server_input_data;
    // std::string server_input_buffer;
};

#endif // CLIENT_HPP