#include <iostream>
#include <vector>
#include <cstdint>
#include <fstream>
#include "socket_lib.h"

using namespace std;

void getFile(int argc, char **argv, vector<char> & output){

	char* fname = argv[argc -1];

    std::ifstream ifs(fname, ios::binary);
    ifs.seekg(0,std::ios::end);
    size_t size = ifs.tellg();
    char * buffer = new char [size];
    ifs.seekg(0,std::ios::beg);
    ifs.read(buffer,size);
    ifs.close();

    output = vector<char>(&buffer[0],buffer + size);
}

int main(int argc, char *argv[]){

	while(1){
		int sock = init_client(8888, inet_addr("127.0.0.1"));
		printf("Connection succes, write what you want to send and press ENTER\n");

		if (sock < 0){
			//Error
			return -1;
		}
		vector<char> output;
    	getFile(argc, argv,output);

		bool res = send_all(sock, &output[0], output.size());
		std::this_thread::sleep_for(std::chrono::seconds(1));
		if (!res)
			break;
		
		close_socket(&sock);
	}

	return 0;
}
