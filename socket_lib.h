#include <string>
#include <netinet/in.h>
#include <resolv.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <functional>
#include <future>

using namespace std;

//Clean and close
void close_socket(int *sock){
	close(*sock);
}

//Fonction for initialize the server mode
void init_server(int port, uint32_t ip,std::function<void(int)> callback){
	try{

		int socket_desc, client_sock, c, new_sock;
		struct sockaddr_in server, client;

		//Create socket
		socket_desc = socket(AF_INET, SOCK_STREAM, 0);
		if (socket_desc == -1) {
			throw "Could not create socket";
		};

		//Prepare the sockaddr_in structure
		server.sin_family = AF_INET;
		server.sin_addr.s_addr = ip;
		server.sin_port = htons(port);

		//Bind
		if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
			throw "bind failed";
		}

		//Listen
		listen(socket_desc, 3);

		//Accept and incoming connection
		c = sizeof(struct sockaddr_in);
		while (1){

			if ((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c))){
				printf("Connection accepted");
                callback(client_sock);
			}
		}
	}
	catch (char const* error_msg){
		printf("%s\n", error_msg);
	}

}

//Fonction for initialize the client mode
int init_client(int port, uint32_t ip){
	try{

		int sock;
		struct sockaddr_in server;

		//Create socket
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
			throw "Could not create socket";
		}

		server.sin_addr.s_addr = ip;
		server.sin_family = AF_INET;
		server.sin_port = htons(port);

		//Connect to remote server
		if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0){
			throw "connect failed";
		}
		return sock;
	}
	catch (char const* error_msg){
		printf("%s\n", error_msg);
		return -1;
	}
}

bool send_all(int sock, char *msg, size_t size){

	try{
		uint32_t length = size;

		size_t n = size;
		size_t idx = 0;

		const uint32_t varSize = size;
		size_t _rsz = send(sock, &varSize, sizeof (varSize), 0);
		while(n > 0){
			size_t rsz = send(sock, &msg[idx], n, 0);
			if (rsz == -1) return false;
			n -= rsz;
			idx += rsz;
			cout << "rsz: " << rsz << endl;
			cout << "n: " << n << endl;
			// cout << idx << endl;
		}
		return true;

	}
	catch (char const* error_msg){
		printf("%s\n", error_msg);
		return false;
	}

}

int recv_all(int sockfd, void *buf, size_t len, int flags)
{
    size_t toread = len;
    char  *bufptr = (char*) buf;

	// cout << "toread: " << toread << endl;
    while (toread > 0)
    {
        size_t rsz = recv(sockfd, bufptr, toread, flags);
        if (rsz <= 0)
            return rsz;

        toread -= rsz;
        bufptr += rsz;
		cout << "rsz: " << rsz << endl;
		cout << "toread: " << toread << endl;
		// cout << strlen((char *)buf) << endl;
    }

    return len;
}
