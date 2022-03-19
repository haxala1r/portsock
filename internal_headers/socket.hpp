#ifndef PORTSOCK_INTERNAL_H
#define PORTSOCK_INTERNAL_H

/* Either __LINUX or __WINDOWS needs to be defined depending on the 
 * *target* operating system by the build system.
 * 
 * i.e. you need to specify on the make command which platform you want
 * to build for.
 */


#ifdef __LINUX
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <unistd.h>

#endif /* __LINUX */

#ifdef __WINDOWS
#include <winsock2.h>
#endif /* __WINDOWS */

/* This class is really only ever used for data storage. */
namespace portsock {
	class SocketInternal {
	private:
	public:
		#ifdef __LINUX
		int sock;
		sockaddr_in bound_addr;
		sockaddr_in conn_addr;
		#endif
		
		#ifdef __WINDOWS
		sockaddr_in bound_addr;
		sockaddr_in conn_addr;
		SOCKET sock;
		#endif
		
		SocketInternal();
		~SocketInternal();
		
		void SetBoundAddr(char *IP, int port);
		void SetConnAddr(char *IP, int port);
		
		void Reset(void);
	};
};

#endif
