// Standard Libraries
#include <cstring>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <vector>


// External Libraries
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <tls.h>
#include <unistd.h>


// Custom Libraries
#include "../components/defs.h"
#include "../components/err.h"
#include "../components/hrw.h"
#include "../components/packet.h"


// Globals: certificate paths
const char* ROOT_PATH = "../../certificates/client_proxy/root.pem";


using namespace std;
int main(int argc, char** argv){
	
	// validate arguments
	exit_on_fail("CLIENT", "Pass in 1 argument", argc == 1);
	int client = atoi(argv[1]);
	exit_on_fail("CLIENT", "Incorrect client number argument", !(client >= 0 && client < CLIENT_NUM));
	cout << "Client starting up! Please be patient..." << endl;


	// buffer initializations
	char recvBuffer[1024];
	char sendBuffer[1024];


	// file initializations
	ifstream request_file;
	ofstream object_file;


	// storage initializations
	vector<string> requests;	
	unordered_map<string, string> objects_data;


	// tls initializations
	struct tls_config* proxy_config = nullptr;
	struct tls* proxy_context  = nullptr;


	// status initializations
	int file_request_status = -1;	
	int tls_proxy_init_status = -1;
	int tls_proxy_root_status = -1;
	int tls_proxy_config_status = -1;
	int tls_proxy_connect_status = -1;
	int tls_proxy_close_status = -1;
	int proxy_socket = -1;
	int proxy_connect_status = -1;
	int proxy_accept_status = -1;	
	int proxy_close_status = -1;
	int tls_write_size = -1;
	int tls_read_size = -1;


	// construct sockaddr storing 
	struct sockaddr_in proxy_address;
	proxy_address.sin_family = AF_INET;
	proxy_address.sin_addr.s_addr = INADDR_ANY;




	// read request objects
	request_file.open(CLIENT_PATH[client]);
	file_request_status = request_file.is_open();
	exit_on_fail("CLIENT", "file_request_status", file_request_status == 0);

	string key;
	int cnt = 0;
	while(request_file >> key)
	{
		requests.push_back(key);
		++cnt;
	}
	request_file.close();

	cout << "Loaded " << cnt << " requests ready to go!" << endl;



	
	// TLS Client initialize (for sending)
	tls_proxy_init_status = tls_init();	
	exit_on_fail("CLIENT", "tls_proxy_init_status", tls_proxy_init_status == -1);

	// TLS Client creates new config for proxy_config
	proxy_config = tls_config_new();
	exit_on_fail("CLIENT", "proxy_config", proxy_config == NULL);

	// TLS Client sets cert based on client_proxy connection
	tls_proxy_root_status = tls_config_set_ca_file(proxy_config, ROOT_PATH);
	exit_on_fail("CLIENT", "tls_proxy_root_status", tls_proxy_root_status == -1);

	
	// start making separate connections for each item to request
	cnt = 0;
	for(auto item: requests)
	{

		// prepare buffer for sending and to send to the appropriate proxy
		int proxy = HRW(item.c_str());
		proxy_address.sin_port = htons(PROXY_PORT[proxy]);
		packet pkt;
		pkt.get(item);	
		++cnt;


		// Create PROXY socket (for sending data)
		proxy_socket = socket(AF_INET, SOCK_STREAM, 0);
		exit_on_fail("CLIENT", "proxy_socket", proxy_socket == -1);

		// Connect PROXY socket
		proxy_connect_status = connect(proxy_socket, (struct sockaddr*)& proxy_address, sizeof(proxy_address));
		exit_on_fail("CLIENT", "proxy_connect_status", proxy_connect_status == -1);




		// TLS proxy_context creation as a tls_client()
		proxy_context = tls_client();
		exit_on_fail("CLIENT", "proxy_context", proxy_context == nullptr);

		// TLS proxy_context configured to config file
		tls_proxy_config_status = tls_configure(proxy_context, proxy_config);
		exit_on_fail("CLIENT", "tls_proxy_config_status", tls_proxy_config_status == -1);

		// TLS proxy_context connects to Proxy
		tls_proxy_connect_status = tls_connect_socket(proxy_context, proxy_socket, "localhost");
		exit_on_fail("CLIENT", "tls_proxy_connect_status", tls_proxy_connect_status == -1);




		// send request to appropriate proxy
		cout << cnt << ") Requesting data for (" << item << ") from proxy" << proxy << endl;
		strcpy(sendBuffer, pkt.c_str());
		tls_write_size = 0;
		tls_write_size = tls_write(proxy_context, sendBuffer, strlen(sendBuffer));

		// read back result from proxy
		tls_read_size = tls_read(proxy_context, recvBuffer, sizeof(recvBuffer) - 1);
		recvBuffer[tls_read_size] = '\0';

		// parse incoming packet
		pkt.receive(recvBuffer, strlen(recvBuffer));	
		if(pkt.getPrefix() == "PUT")
		{
			cout << "\tReceived data for (" << item << ") -> {" << pkt.getMsg() << "}" << endl;
		}
	
		else if(pkt.getPrefix() == "NON")
		{
			cout << "\tReceived data for (" << item << ") -> " << "[Nonexistent Object]" << endl;
		}	

		else if(pkt.getPrefix() == "DEN")
		{
			cout << "\tReceived data for (" << item << ") -> " << "[Blacklisted Object]" << endl;
		}

		else
		{
			exit_on_fail("CLIENT", "pkt.receive", true);
		}




		// tls close socket
		tls_proxy_close_status = 0;
		do {
			tls_proxy_close_status = tls_close(proxy_context);
		} while(tls_proxy_close_status == TLS_WANT_POLLIN || tls_proxy_close_status == TLS_WANT_POLLOUT);
		
		// close PROXY socket	
		proxy_close_status = close(proxy_socket);	
		exit_on_fail("CLIENT", "proxy_close_status", proxy_close_status == -1);
	}	
}



