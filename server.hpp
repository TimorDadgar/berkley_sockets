#ifndef SERVER_HPP
#define SERVER_HPP

#include <winsock2.h>
#include <ws2tcpip.h>
#include <memory>

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
private:
    friend class SocketUtil;
    // Have constructor like this if using SocketUtil
    // UDPSocket(SOCKET inSocket) : mSocket(inSocket) {};
    SOCKET mSocket;
};
typedef std::shared_ptr<UDPSocket> UDPSocketPtr;

int start_pong_game();
int berkley_socket();

#endif // SERVER_HPP
