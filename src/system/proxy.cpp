// Standard Libraries
#include <cstring>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
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
#include "../components/bf.h"



// Globals: certificate paths
const char* ACCEPT_ROOT_PATH = "../../certificates/client_proxy/root.pem";
const char* ACCEPT_CERT_PATH = "../../certificates/client_proxy/server.crt";
const char* ACCEPT_KEY_PATH = "../../certificates/client_proxy/server.key";

const char* CONNECT_ROOT_PATH = "../../certificates/server_proxy/root.pem";


using namespace std;
int main(int argc, char** argv)
{
	/*
	 * Initialize variables and 
	 * data structures to be used
	 */

	// validate arguments
	exit_on_fail("PROXY", "Pass in 1 argument", argc == 1);
	int proxy = atoi(argv[1]);
	exit_on_fail("PROXY", "Incorrect proxy number argument", !(proxy >= 0 && proxy < PROXY_NUM));
	cout << "Proxy" << proxy << " is starting up! Please be patient." << endl;


	// buffer initializations
	char recvBuffer[1024];
	char sendBuffer[1024];


	// file descriptor initialization
	int fd[2];
	

	// file initializations
	ifstream blacklist_file;


	// storage initializations
	unordered_map<string, string> cache;
	unordered_set<string> blacklist;


	// tls initializations
	struct tls_config* proxy_config = nullptr;
	struct tls_config* server_config = nullptr;
	struct tls* client_context = nullptr;
	struct tls* proxy_context = nullptr;
	struct tls* server_context = nullptr;


	// status initializations
	int file_blacklist_status = -1;
	int tls_proxy_init_status = -1;
	int tls_proxy_root_status = -1;
	int tls_proxy_cert_status = -1;
	int tls_proxy_key_status = -1;
	int tls_proxy_config_status = -1;
	int tls_proxy_accept_status = -1;
	int tls_server_init_status = -1;
	int tls_server_root_status = -1;
	int tls_server_config_status = -1;
	int tls_server_connect_status = -1;
	int tls_server_close_status = -1;
	int tls_client_close_status = -1;
	int listen_socket = -1;
	int listen_bind_status = -1;
	int listen_status = -1;
	int proxy_socket = -1;
	int proxy_accept_status = -1;
	int proxy_close_status = -1;
	int server_socket = -1;
	int server_connect_status = -1;
	int server_accept_status = -1;
	int server_close_status = -1;
	int client_socket = -1;
	int tls_write_size = -1;
	int tls_read_size = -1;
	int fd_pipe_status = -1;
	int fd_write_status = -1;
	int fd_read_status = -1;
	int fd_close_status = -1;


	
	// client_address will be for responding to clients by forking for incoming connections
	struct sockaddr_in client_address;


	// proxy_address will be for listening to incoming connections from clients
	struct sockaddr_in proxy_address;
	proxy_address.sin_family = AF_INET;
	proxy_address.sin_port = htons(PROXY_PORT[proxy]);
	proxy_address.sin_addr.s_addr = INADDR_ANY;


	// server_address will be for sending and receiving data to the server
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(SERVER_PORT);
	server_address.sin_addr.s_addr = INADDR_ANY;


	// read blacklist objects into set & bloom filter
	blacklist_file.open(PROXY_PATH[proxy]);
	file_blacklist_status = blacklist_file.is_open();
	exit_on_fail("PROXY", "file_blacklist_status", file_blacklist_status == 0);

	string key;
	int cnt = 0;
	while(blacklist_file >> key)
	{
		++cnt;
		blacklist.insert(key);
	}

	blacklist_file.close();

	// initialize bloom filter	
	cout << "Initializing " << cnt << " items on the bloom filter..." << endl;
	bloom_filter filter;
	filter.initialize(blacklist);	




	// server_context initialized for CONNECT to server
	cout << "Preparing TLS Connections..." << endl;
	tls_server_init_status = tls_init();
	exit_on_fail("PROXY", "tls_server_init_status", tls_server_init_status == -1);

	server_config = tls_config_new();
	exit_on_fail("PROXY", "server_config", server_config == nullptr);

	tls_server_root_status = tls_config_set_ca_file(server_config, CONNECT_ROOT_PATH);
	exit_on_fail("PROXY", "tls_server_root_status", tls_server_root_status == -1);	




	// proxy_context initialized to ACCEPT client connections
	proxy_config = tls_config_new();
	exit_on_fail("PROXY", "proxy_config", proxy_config == nullptr);

	tls_proxy_root_status = tls_config_set_ca_file(proxy_config, ACCEPT_ROOT_PATH);
	exit_on_fail("PROXY", "tls_proxy_root_status", tls_proxy_root_status == -1);

	tls_proxy_cert_status = tls_config_set_cert_file(proxy_config, ACCEPT_CERT_PATH);
	exit_on_fail("PROXY", "tls_proxy_cert_status", tls_proxy_cert_status == -1);

	tls_proxy_key_status = tls_config_set_key_file(proxy_config, ACCEPT_KEY_PATH);
	exit_on_fail("PROXY", "tls_proxy_key_status", tls_proxy_key_status == -1);

	proxy_context = tls_server();
	exit_on_fail("PROXY", "proxy_context", proxy_context == nullptr);

	tls_proxy_config_status = tls_configure(proxy_context, proxy_config);
	exit_on_fail("PROXY", "tls_proxy_config_status", tls_proxy_config_status == -1);
		



	// create LISTEN socket
	cout << "Listening for Client Connections..." << endl;
	listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	exit_on_fail("PROXY", "listen_socket", listen_socket == -1);

	// bind LISTEN socket to local server_address
	listen_bind_status = bind(listen_socket, (struct sockaddr*)& proxy_address, sizeof(proxy_address));	
	exit_on_fail("PROXY", "listen_bind_status", listen_bind_status == -1);

	// listen for packets using the LISTEN socket
	listen_status = listen(listen_socket, MAX_CONNECTIONS);
	exit_on_fail("PROXY", "listen_status", listen_status == -1);




	/*
	 * Waiting for GET requests from the client
	 * if blacklist, return DEN
	 * if not found, return NON
	 * if found, return item
	 */
	
	cout << "Ready to accept Client Requests!" << endl;
	int client_pid = 0;
	socklen_t client_len = sizeof(&client_address); // note here if issue in future
	
	while(1)
	{
		// accept client connection as client socket
		client_socket = accept(listen_socket, (struct sockaddr*)& client_address, &client_len);
		exit_on_fail("PROXY", "client_socket", client_socket == -1);

		// open pipe for child-parent process interaction
		fd_pipe_status = pipe(fd);
		exit_on_fail("PROXY", "fd_pipe_status", fd_pipe_status == -1);

		// fork process to handle multiple clients
		client_pid = fork();
		exit_on_fail("PROXY", "client_pid", client_pid == -1);

		// if child process, handle incoming connection
		if(client_pid == 0)
		{
			// close the reading part of pipe
			fd_close_status = close(fd[0]);
			exit_on_fail("PROXY", "fd_close_status fd[0]", fd_close_status == -1);


			// accept tls connection
			tls_proxy_accept_status = tls_accept_socket(proxy_context, &client_context, client_socket);
			exit_on_fail("PROXY", "tls_proxy_accept_status", tls_proxy_accept_status == -1);


			// read data from client
			tls_read_size = -1;
			tls_read_size = tls_read(client_context, recvBuffer, sizeof(recvBuffer) - 1);
			recvBuffer[tls_read_size] = '\0';


			// check if object received is in blacklist
			packet pkt;
			pkt.receive(recvBuffer, strlen(recvBuffer));
			if(pkt.getPrefix() == "GET")
			{
				string objname = pkt.getMsg();

				// if in bloom filter and blacklist, return "DEN"
				if(filter.check(objname.c_str()) && blacklist.find(objname) != blacklist.end())
				{
					cout << "NEW Request for (" << objname << ") found on Bloom Filter or Blacklist -> Returning [DEN] to Client" << endl;
					pkt.den();
				}
				
				// if in cache, then we prepare packet with object
				else if(cache.find(objname) != cache.end())
				{
					cout << "NEW Request for (" << objname << ") found in Cache ->  Returning to Client" << endl;
					pkt.put(cache[objname]);
				}


				// if not in cache, make a request to the server
				else
				{

					cout << "NEW Request for (" << objname << ") is not on Cache -> Contacting Server" << endl;

					// create SERVER socket (for sending data)
					server_socket = socket(AF_INET, SOCK_STREAM, 0);
					exit_on_fail("PROXY", "server_socket", server_socket == -1);

					// CONNECT server socket
					server_connect_status = connect(server_socket, (struct sockaddr*)& server_address, sizeof(server_address));
					exit_on_fail("PROXY", "server_connect_status", server_connect_status == -1);




					// TLS server_context creation as a tls_client()
					server_context = tls_client();
					exit_on_fail("PROXY", "server_context", server_context == nullptr);

					// TLS server_context configured to config file
					tls_server_config_status = tls_configure(server_context, server_config);
					exit_on_fail("PROXY", "tls_server_config_status", tls_server_config_status == -1);

					// TLS server_context connects to Server
					tls_server_connect_status = tls_connect_socket(server_context, server_socket, "localhost");
					exit_on_fail("PROXY", "tls_server_connect_status", tls_server_connect_status == -1);




					// forward client request to server
					strcpy(sendBuffer, pkt.c_str());
					tls_write_size = tls_write(server_context, sendBuffer, strlen(sendBuffer));

					// read incoming response from server
					tls_read_size = -1;
					tls_read_size = tls_read(server_context, recvBuffer, sizeof(recvBuffer) - 1);
					recvBuffer[tls_read_size] = '\0';
					pkt.receive(recvBuffer, strlen(recvBuffer));
					cout << "\tReceived response for (" << objname << ") from Server -> Returning to Client" << endl;

					// send server's response to parent process cache if valid
					if(pkt.getPrefix() == "PUT")
					{
						pair<string, string> cache_pair; 
						cache_pair.first = objname;
						cache_pair.second = pkt.getMsg();

						// write to file descriptor
						fd_write_status = write(fd[1], &cache_pair, sizeof(cache_pair));
						exit_on_fail("PROXY", "fd_write_status fd[1]", fd_write_status == -1);

						// close writing file descriptor
						fd_close_status = close(fd[1]);
						exit_on_fail("PROXY", "fd_close_status fd[1]", fd_close_status == -1);
					}		
				}
			}

			// don't handle if non-GET request from client
			else
			{
				exit_on_fail("PROXY", "pkt.receive from client", true);
			}

	

			// respond to client with appropriate packet contents
			strcpy(sendBuffer, pkt.c_str());	
			tls_write_size = tls_write(client_context, sendBuffer, strlen(sendBuffer));

			// close this connection
			tls_client_close_status = 0;
			do {
				tls_client_close_status = tls_close(client_context);
			} while(tls_client_close_status == TLS_WANT_POLLIN || tls_client_close_status == TLS_WANT_POLLOUT);
			close(client_socket);
			exit(0);
		}

		pair<string, string> cache_data;


		// parent process needs to grab server loaded info if there is any
		fd_close_status = close(fd[1]);
		exit_on_fail("PROXY", "fd_close_status fd[1]", fd_close_status == -1);


		// read the pair from child process file descriptor
		fd_read_status = read(fd[0], &cache_data, sizeof(cache_data));
		exit_on_fail("PROXY", "fd_read_status fd[0]", fd_read_status == -1);


		// close reading file descriptor for parent
		fd_close_status = close(fd[0]);
		exit_on_fail("PROXY", "fd_close_status fd[0]", fd_close_status == -1);


		// read if pulled something from file descriptor
		if(cache_data.first != "")
		{
			cache[cache_data.first] = cache_data.second;
			cout << "\tUpdating Cache with pair: (" << cache_data.first << " , " << cache_data.second << ")" <<  endl;
		}
		
	}
}


