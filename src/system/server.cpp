// Standard Libraries
#include <cstring>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unordered_map>

// External Libraries
#include <netinet/in.h>
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
const char* ROOT_PATH = "../../certificates/server_proxy/root.pem";
const char* CERT_PATH = "../../certificates/server_proxy/server.crt";
const char* KEY_PATH = "../../certificates/server_proxy/server.key";


using namespace std;
int main(){

	cout << "Server is starting up! Please be patient." << endl;

	// buffer initializations
	char recvBuffer[1024];
	char sendBuffer[1024];


	// file initializations
	ifstream object_file;
	ifstream blacklist_file;
	ofstream proxy_file[PROXY_NUM];


	// storage initializations
	unordered_map<string, string> object_db; 
	unordered_map<string, string> blacklist_db;


	// tls initializations
	struct tls_config* server_config = nullptr;
	struct tls* server_context = nullptr;
	struct tls* proxy_context = nullptr;


	// status initializations
	int file_object_status = -1;
	int file_blacklist_status = -1;
	int file_proxy_status = -1;
	int tls_server_root_status = -1;
	int tls_server_cert_status = -1;
	int tls_server_key_status = -1;
	int tls_server_config_status = -1;
	int tls_server_accept_status = -1;
	int tls_server_close_status = -1;
	int listen_socket = -1;
	int listen_bind_status = -1;
	int listen_status = -1;
	int proxy_socket = -1;
	int proxy_accept_status = -1;
	int proxy_close_status = -1;
	int tls_read_size = -1;
	int tls_write_size = -1;


	// declare sockaddr proxy_address that will be used by all proxies when forking connections
	struct sockaddr_in proxy_address;

	// construct sockaddr storing server address
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(SERVER_PORT);
	server_address.sin_addr.s_addr = INADDR_ANY;




	// read objects and store items into object_db

	object_file.open(OBJECT_PATH);
	file_object_status = object_file.is_open();
	exit_on_fail("SERVER", "file_object_status", file_object_status == 0);

	string key, value;
	int cnt = 0;
	while(object_file >> key >> value)
	{
		++cnt;
		object_db[key] = value;
	}
	object_file.close();

	cout << "Loaded " << cnt << " requestable objects onto the server..." << endl;





	// read blacklist and store items into blacklist_db
	blacklist_file.open(BLACKLIST_PATH);
	file_blacklist_status = blacklist_file.is_open();
	exit_on_fail("SERVER", "file_blacklist_status", file_blacklist_status == 0);

	cnt = 0;
	while(blacklist_file >> key >> value)
	{
		blacklist_db[key] = value;
		++cnt;
	}		
	blacklist_file.close();

	cout << "Loaded " << cnt << " blacklisted objects onto the server..." << endl;




	// write blacklist items as files for proxies to use
	cout << "Distributing blacklisted items across proxies..." << endl;
	for(int i = 0; i < PROXY_NUM; ++i)
	{
		proxy_file[i].open(PROXY_PATH[i]);
		file_proxy_status = proxy_file[i].is_open();	
		exit_on_fail("SERVER", "file_proxy_status", file_proxy_status == 0);
	}	

	for(auto item : blacklist_db)
	{
		int proxy = HRW(item.first.c_str());
		proxy_file[proxy] << item.first << endl;	
	}	

	for(int i = 0; i < PROXY_NUM; ++i)
	{
		proxy_file[i].close();
	}




	// TLS Server Create New Config
	cout << "Preparing TLS Connections..." << endl;
	server_config = tls_config_new();
	exit_on_fail("SERVER", "server_config", server_config == nullptr);

	// TLS Server Root File
	tls_server_root_status = tls_config_set_ca_file(server_config, ROOT_PATH);
	exit_on_fail("SERVER", "tls_server_root_status", tls_server_root_status == -1); 

	// TLS Server Cert File
	tls_server_cert_status = tls_config_set_cert_file(server_config, CERT_PATH);
	exit_on_fail("SERVER", "tls_server_cert_status", tls_server_cert_status == -1);

	// TLS Server Key File
	tls_server_key_status = tls_config_set_key_file(server_config, KEY_PATH);
	exit_on_fail("SERVER", "tls_server_key_status", tls_server_key_status == -1);

	// TLS Server Creation
	server_context = tls_server();
	exit_on_fail("SERVER", "server_context", server_context == nullptr);
	
	// TLS Server Configuration with Server Context
	tls_server_config_status = tls_configure(server_context, server_config);
	exit_on_fail("SERVER", "tls_server_config_status", tls_server_config_status == -1);




	// create LISTEN socket
	listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	exit_on_fail("SERVER", "listen_socket", listen_socket == -1);

	// bind LISTEN socket to local server_address
	listen_bind_status = bind(listen_socket, (struct sockaddr*)& server_address, sizeof(server_address));
	exit_on_fail("SERVER", "listen_bind_status", listen_bind_status == -1);

	// listen for packets using the LISTEN socket
	listen_status = listen(listen_socket, MAX_CONNECTIONS);
	exit_on_fail("SERVER", "listen_status", listen_status == -1);
	


	/*
	 * Waiting for GET requests from the proxy
	 * and if found, then return item.
	 * If not found, then return NON.
	 */	


	cout << "Ready to accept Proxy Requests!" << endl;
	int proxy_pid = 0;
	socklen_t proxy_len = sizeof(&proxy_address); // note here if issue

	while(1)
	{
		
		// accept proxy connection as proxy socket
		proxy_socket = accept(listen_socket, (struct sockaddr*)& proxy_address, &proxy_len);
		exit_on_fail("SERVER", "proxy_socket", proxy_socket == -1);




		// fork process to handle multiple proxies
		proxy_pid = fork();
		exit_on_fail("SERVER", "proxy_pid", proxy_pid == -1);

		// if child process, handle incoming connection
		if(proxy_pid == 0)
		{		

			// accept tls connection
			tls_server_accept_status = tls_accept_socket(server_context, &proxy_context, proxy_socket);
			exit_on_fail("SERVER", "tls_server_accept_status", tls_server_accept_status);	


			// read incoming message from proxy into recvBuffer
			tls_read_size = tls_read(proxy_context, recvBuffer, sizeof(recvBuffer) - 1);
			recvBuffer[tls_read_size] = '\0';


			// check if object received is in db
			// pkt will hold what to send back to proxy
			packet pkt;
			pkt.receive(recvBuffer, strlen(recvBuffer));

			if(pkt.getPrefix() == "GET")
			{
				string objname = pkt.getMsg();	
				if(object_db.find(objname) != object_db.end())
				{
					cout << "NEW Request for (" << objname << ") FOUND -> Returning to Proxy" << endl;
					pkt.put(object_db[objname]);
				}
				else
				{
					cout << "NEW Request for " << objname << " NOT FOUND -> returning [NON] to Proxy" << endl;
					pkt.non();
				}

			}

			else
			{
				exit_on_fail("SERVER", "pkt.receive", true);
			}




			// respond to sender with contents of packet (PUT or NON)
			strcpy(sendBuffer, pkt.c_str());
			tls_write_size = tls_write(proxy_context, sendBuffer, strlen(sendBuffer));


			// close this connection
			tls_server_close_status = 0;
			do {
				tls_server_close_status = tls_close(proxy_context);
			} while(tls_server_close_status == TLS_WANT_POLLIN || tls_server_close_status == TLS_WANT_POLLOUT);
			close(proxy_socket);
			exit(0);
		}
	}
}

