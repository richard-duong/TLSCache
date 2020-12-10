// Standard Libraries
#include <cstring>
#include <iostream>
#include <fstream>
#include <stdio>
#include <stdlib>
#include <string>
#include <unordered_map>
#include <vector>


// External Libraries
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <tls.h>
#include <unistd.h>


// Custom Libraries
#include "../components/defs.h"
#include "../components/packet.h"
#include "../components/err.h"
#include "../components/HRW.h"

// Globals: certificate paths
const char* ROOT_PATH = "../../certificates/client_proxy/root.pem";

using namespace std;

int main(int argc, char** argv){

	// buffer initializations
	char recvBuffer[1024];
	char sendBuffer[1024];

	// tls initializations
	struct tls_config* proxy_config = nullptr;
	struct tls* proxy_context = nullptr;

	// status initializations
	int file_request_status = -1;	
	int tls_client_init_status = -1;
	int tls_client_root_status = -1;
	int proxy_socket = -1;
	int proxy_accept_status = -1;	
	int proxy_close_status = -1;
	int tls_proxy_accept_status = -1;
	int tls_proxy_close_status = -1;

	// file initializations
	ifstream request_file;

	// storage initializations
	vector<string> objects;
	unordered_map<string, string> objects_data;

	// construct sockaddr storing 
	struct sockaddr_in proxy_address;
	proxy_address.sin_family = AF_INET;
	proxy_address.sin_addr.s_addr = INADDR_ANY;




}


























