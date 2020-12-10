// Standard Libraries
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>

// External Libraries
#include <tls.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

using namespace std;

int main(){

	/* Initialize */
	char buffer[1024];
	struct sockaddr_in sockname;
	struct sockaddr_in client;

	struct tls_config* tls_cfg = NULL;
	struct tls* tls_ctx;
	struct tls* tls_cctx;

	int port = 9999;
	int listen_socket = -1;
	int clientsd = -1;
	int pid = -1;
	socklen_t clientlen;

	/* Setup TLS */
	if((tls_cfg = tls_config_new()) == NULL)
		cout << "Error: Unable to allocate TLS config" << endl;

	if((tls_config_set_ca_file(tls_cfg, "../../certificates/root.pem")) == -1)
		cout << "Error: Unable to set root CA file" << endl;

	if((tls_config_set_cert_file(tls_cfg, "../../certificates/server.crt")) == -1)
		cout << "Error: Unable to set TLS certificate" << endl;

	if(tls_config_set_key_file(tls_cfg, "../../certificates/server.key") == -1)
		cout << "Error: Unable to set TLS key file" << endl;

	if((tls_ctx = tls_server()) == NULL)
		cout << "Error: Unable to create server creation" << endl;

	if(tls_configure(tls_ctx, tls_cfg) == -1)
		cout << "Error: TLS Configuration failed" << endl;

	

	/* Get sock struct ready */
	memset(&sockname, 0, sizeof(sockname));
	sockname.sin_family = AF_INET;
	sockname.sin_port = htons(port);
	sockname.sin_addr.s_addr = htonl(INADDR_ANY);

	/* Create client dynamically here
	 * Fork here
	 */
	
	/* Set up listening socket */
	listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_socket == -1)
		cout << "Error: Create a socket" << endl;		
	if(bind(listen_socket, (struct sockaddr*)& sockname, sizeof(sockname)) == -1)
		cout << "Error: Bind failed" << endl;
	if(listen(listen_socket, 3) == -1)
		cout << "Error: Listen failed" << endl;


	/* Set up constant listening*/
	pid = -1;
	while(true){
		clientlen = sizeof(client);
		clientsd = accept(listen_socket, (struct sockaddr*) &client, &clientlen);
		if(clientsd == -1)
			cout << "Error: Accept failed" << endl;

		pid = fork();
		if(pid == -1)
			cout << "Error: Fork failed" << endl;

		if(pid == 0){
			ssize_t written, w;
			int i = 0;

			if(tls_accept_socket(tls_ctx, &tls_cctx, clientsd) == -1){
				cout << "Error: TLS Accept failed" << endl;
			}
			else{
				do{
					if((i = tls_handshake(tls_cctx)) == -1)
						cout << "Error: TLS Handshake Failed" << endl;
				} while(i == TLS_WANT_POLLIN || i == TLS_WANT_POLLOUT);
			}


			ssize_t rc = 0;
		   	ssize_t	r = -1;
			size_t maxread = sizeof(buffer) - 1;

			while((r != 0) && rc < maxread){
				r = tls_read(tls_cctx, buffer + rc, maxread - rc);
				if(r == TLS_WANT_POLLIN || r == TLS_WANT_POLLOUT)
					continue;
				if(r < 0)
					cout << "Error: TLS Read Failed" << endl;
				else
					rc += r;
				cout << "reading";
			}

			cout << "Result: ";
			cout << buffer << endl;
			

			/*
			w = 0;
			written = 0;
			while(written < strlen(buffer)){
				w = tls_write(tls_cctx, buffer + written, strlen(buffer) - written);
				if(w == -1 && errno != EINTR){
					cout << "Error: TLS Write failed" << endl;
				}
				else{
					written += w;
				}
			}
			*/

			close(clientsd);
			exit(0);
		}
		close(clientsd);
	}
}

