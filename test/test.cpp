#include <iostream>
#include "../headers/socket.hpp"

using namespace std;
using namespace portsock;

int main(void) {
	/* This is a simple test function 
	 * TODO: have some actual tests?
	 */
	Socket s1;
	string ip = "127.0.0.1";
	int port = 6666;
	
	try {
		if (s1.Connect(ip, port)) {
			throw "Connect fails";
		}
	} catch (const char *e) {
		cout << e;
	}
	
	return 0; 
}
