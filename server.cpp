#include <cstdio>
#include <iostream>
#include <fstream>
#include <functional>
#include "socket_lib.h"

using namespace std;

void client_thread(int socket){

	printf("Client connected\n");

	uint32_t size;
	int len = recv(socket,(char*)&size, 4, 0);
	char *buf = new char[size];
	int res = recv_all(socket, buf, size, 0);

	// ofstream ofs("_img.png", ios::binary);
	// ofs.write(buf, size);
	// ofs.close();

	return;
}

int main(int argc, char *argv[]){
    std::cout << "test" << std::endl;

    init_server(8888, inet_addr("127.0.0.1"), client_thread);
    return 0;
}