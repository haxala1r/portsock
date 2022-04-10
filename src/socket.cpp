#include <iostream>
#include "../headers/socket.hpp"
#include "../internal_headers/socket.hpp"

using namespace std;

using namespace portsock;

Socket::Socket(void) {
	internal = new SocketInternal();
	timeout = 0;
	state = Disconnected;
}

Socket::~Socket(void) {
	delete internal;
}

int Socket::Connect(string IP, int port) {
	if (state != Disconnected) return -1;
	SocketInternal *i = internal;
	
	i->SetConnAddr((char*)IP.c_str(), port);

	if (connect(i->sock, (struct sockaddr*)&i->conn_addr, sizeof(i->conn_addr)) < 0) {
		return -1;
	}
	
	state = Connected;
	return 0;
}

int Socket::Disconnect(void) {
	if (state == Disconnected) return 0;
	
	internal->Reset();
	state = Disconnected;
	return 0;
}

int Socket::Listen(std::string IP, int port) {
	if (internal == nullptr) return -1;
	if (state != Disconnected) return -1;
	
	state = Listening;
	internal->SetBoundAddr((char*)IP.c_str(), port);
	if (bind(internal->sock, (struct sockaddr*)&internal->bound_addr, sizeof(internal->bound_addr)) < 0) {
		Disconnect();
		return -2;
	}
	if (listen(internal->sock, 10) < 0) {
		Disconnect();
		return -3;
	}
	
	return 0;
}

/* Checks if the socket currently has a connection that can be accepted */
bool Socket::CheckRead(void) {
	if (timeout == 0) return true;
	
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(internal->sock, &fds);
	
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = timeout;
	int stat = select(internal->sock + 1, &fds, nullptr, nullptr, &tv);
	FD_ZERO(&fds);
	if (stat > 0) {
		return true;
	} else if (stat < 0) {
		/* Error. TODO: It may not be a good idea to use C++ exceptions
		 * here... maybe use return values instead? although that may 
		 * become confusing very quickly.
		 */
		Disconnect();
		throw "Cannot select()";
	}
	
	
	return false;
}

Socket *Socket::Accept(void) {
	if (state != Listening) return nullptr;
	if (!CheckRead()) return nullptr;
	
	Socket *ns = new Socket();
	
	#ifdef __LINUX
	close(ns->internal->sock);
	socklen_t len = sizeof(ns->internal->conn_addr);
	#endif
	#ifdef __WINDOWS
	closesocket(ns->internal->sock);
	int len = sizeof(ns->internal->conn_addr);
	#endif
	
	ns->internal->sock = accept(internal->sock, (struct sockaddr *)&ns->internal->conn_addr, &len);
	ns->state = Connected;
	ns->timeout = timeout;
	
	return ns;
}

int Socket::SendStr(std::string s) {
	return this->Send((void*)s.c_str(), (int)s.length() + 1);
}

/* Send an area of memory, like the actual send() system call */
int Socket::Send(void *buf, int len) {
	if (state != Connected) return -1;
	
	int status = (int)send(internal->sock, (char*)buf, len, 0);
	if (status <= 0) {
		Disconnect();
	}
	return status;
}

/* This is horribly inefficient, and should not be used for moving
 * tons of data. Use Recv() instead if efficiency is desired.
 */
std::string Socket::RecvStr(void) {
	string s = "";
	if (state != Connected) return s;
	
	char c;
	while (CheckRead() == true) {
		/* If select() tells us there is data to read but recv doesn't
		 * return any data, there *must* be an error. 
		 */
		if (recv(internal->sock, &c, 1, 0) != 1) {
			Disconnect();
			return s;
		}
		s += c;
	}
	
	return s;
}

int Socket::Recv(void *buf, int max_len) {
	if (state != Connected) return -1;
	if (!CheckRead()) return 0;
	
	int status = (int)recv(internal->sock, (char*)buf, max_len, 0);
	if (status <= 0) {
		Disconnect();
	}
	
	return status;
}

/* This is an extended version of the CheckRead function 
 * returned vector holds the sockets that can be read from
 * NOTE: this is a WIP
 */
vector<Socket> *PollSockets(vector<Socket> vs, int timeout) {
	fd_set fds;
	FD_ZERO(&fds);
	
	for (unsigned int i = 0; i < vs.size(); i++) {
		FD_SET(vs[i].internal->sock, &fds);
	}
	
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = timeout;
	int stat = select(internal->sock + 1, &fds, nullptr, nullptr, &tv);
	
	vector<Socket> *retv = new vector<Socket>;
	if (stat >= 0) {
		for (unsigned int i = 0; i < vs.size(); i++) {
			if (FD_ISSET(vs[i].internal->sock, &fds)) {
				retv->push_back(vs[i]);
			}
		}
	}
	
	FD_ZERO(&fds);
	if (stat < 0) {
		throw "Cannot select()";
	}
	return retv;
}

