#ifndef PORTSOCK_H
#define PORTSOCK_H

#include <vector> 

namespace portsock {
	enum SocketState {
		Disconnected=0,
		Connected,
		Listening
	};
	
	class SocketInternal;
	class Socket {
	public:
		/* We need to keep this in a pointer because the class
		 * we use here would actually change depending on which platform
		 * we're running. I'm not very well versed in C++, so if there
		 * is any way of doing this in a less C-like way, tell me.
		 */
		SocketInternal *internal;
		
		/* Timeout is in microseconds. This may change later.*/
		SocketState state;
		int timeout;
		
		Socket(void);
		~Socket(void);
		
		/* connection management */
		int Connect(std::string IP, int port);
		int Disconnect(void);
		
		int Listen(std::string IP, int port);
		bool CheckRead(void);
		Socket *Accept(void);
		
		/* Data handling */
		int SendStr(std::string s);
		int Send(void *buf, int len);
		
		std::string RecvStr(void);
		int Recv(void *buf, int len);
		
	};
	std::vector<Socket> *PollSockets(std::vector<Socket> vs, int timeout);
}



#endif
