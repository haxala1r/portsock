#include <iostream>
#include <cstring>
#include "../headers/socket.hpp"
#include "../internal_headers/socket.hpp"

using namespace std;
namespace portsock {
	#ifdef __WINDOWS
	bool initialised = false;
	WSADATA wsa;
	#endif
};
using namespace portsock;



SocketInternal::SocketInternal(void) {
	/* The windows winsock2 library needs to be initialised before being
	 * used. So if it's being compiled for windows, initialise.
	 */
	#ifdef __WINDOWS
	if (initialised == false) {
		if (WSAStartup(MAKEWORD(2,2), &wsa) < 0) {
			throw "CANNOT INITIALISE WINSOCK";
		}
		initialised = true;
	}
	#endif
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock <= 0) throw "Cannot create socket.";
	
	memset(&conn_addr, 0, sizeof(conn_addr));
	memset(&bound_addr, 0, sizeof(bound_addr));
}


SocketInternal::~SocketInternal(void) {
	#ifdef __LINUX
	close(sock);
	#endif
	#ifdef __WINDOWS
	closesocket(sock);
	#endif
}


void SocketInternal::SetConnAddr(char *IP, int port) {
	memset(&conn_addr, 0, sizeof(conn_addr));
	
	conn_addr.sin_family = AF_INET;
	conn_addr.sin_port = htons((short unsigned int)port);
	
	#ifdef __LINUX
	inet_pton(AF_INET, IP, &conn_addr.sin_addr);
	#endif
	
	#ifdef __WINDOWS
	conn_addr.sin_addr.s_addr = inet_addr(IP);
	#endif
}

void SocketInternal::SetBoundAddr(char *IP, int port) {
	memset(&bound_addr, 0, sizeof(conn_addr));
	
	bound_addr.sin_family = AF_INET;
	bound_addr.sin_port = htons((short unsigned int)port);
	
	#ifdef __LINUX
	inet_pton(AF_INET, IP, &bound_addr.sin_addr);
	#endif
	
	#ifdef __WINDOWS
	bound_addr.sin_addr.s_addr = inet_addr(IP);
	#endif
}

void SocketInternal::Reset(void) {
	#ifdef __LINUX
	close(sock);
	#endif
	#ifdef __WINDOWS
	closesocket(sock);
	#endif
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock <= 0) throw "Cannot create socket.";
	
	memset(&conn_addr, 0, sizeof(conn_addr));
	memset(&bound_addr, 0, sizeof(bound_addr));
}
