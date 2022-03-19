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
	#ifdef __LINUX
	if (connect(i->sock, (struct sockaddr*)&i->conn_addr, sizeof(i->conn_addr)) < 0) {
		return -1;
	}
	#endif
	
	#ifdef __WINDOWS
	
	#endif
	
	state = Connected;
	return 0;
}

int Socket::Disconnect(void) {
	if (state == Disconnected) return 0;
	if (state != Connected) return 0;
	
	internal->Reset();
	state = Disconnected;
	return 0;
}

int Socket::Listen(std::string IP, int port) {
	if (internal == nullptr) return -1;
	if (state != Disconnected) return -1;
	
	internal->SetBoundAddr((char*)IP.c_str(), port);
	if (bind(internal->sock, (struct sockaddr*)&internal->bound_addr, sizeof(internal->bound_addr)) < 0) {
		internal->Reset();
		throw "Cannot bind to the given address.";
	}
	if (listen(internal->sock, 10) < 0) {
		internal->Reset();
		throw "Cannot listen on the bound address.";
	}
	
	state = Listening;
	return 0;
}

/* Checks if the socket currently has a connection that can be accepted */
bool Socket::CheckRead(void) {
	if (timeout == 0) return true;
	
	#ifdef __LINUX
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(internal->sock, &fds);
	
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = timeout;
	int stat = select(internal->sock + 1, &fds, nullptr, nullptr, &tv);
	if (stat > 0) {
		FD_ZERO(&fds);
		return true;
	} else if (stat < 0) {
		/* Error */
		Disconnect();
		throw "Cannot select()";
	}
	FD_ZERO(&fds);
	#endif
	
	#ifdef __WINDOWS
	
	#endif
	
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


int main(void) {
	try {
	Socket s;
	cout << "Hello World!\n";
	string ip;
	cin >> ip;
	int port;
	cin >> port;
	
	s.timeout = 1000000;
	s.Listen(ip, port);

	Socket *ns = nullptr;
	while (ns == nullptr) {
		ns = s.Accept();
		cout << "loop\n";
	}
	
	cout << ns->RecvStr();
	delete ns;
	} catch (const char *s) {
		cout << s << endl;
	}
	return 0; 
}
