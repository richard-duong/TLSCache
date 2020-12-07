// Standard Libraries
#include <iostream>

// External Libraries
#include "tls.h"
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

using namespace std;

int main(){

	/* Initialize */
	struct sockadrr_in sockname;
	struct tls_config* tls_cfg = NULL;
	int port = 9999;
	int listen_socket = -1;

	/* Setup TLS */
	if((tls_cfg = tls_config_new()) == NULL)
		cout << "Error: Unable to allocate TLS config" << endl;

	if((tls_config_set_ca_file(tls_cfg, "../certs/root.pem")) == -1)
		cout << "Error: Unable to set root CA file" << endl;

	if((tls_config_set_cert_file(tls_cfg, "../certs/server.crt")) == -1)
		cout << "Error: Unable to set TLS certificate" << endl;

	if(tls_config_set_key_file(tls_cfg, "../certs/server.key") == -1)
		cout << "Error: Unable to set TLS key file" << endl;

	if((tls_ctx == tls_config_new()) == NULL)
		cout << "Error: Unable to create server creation" << endl;

	if(tls_configure(tls_ctx, tls_cfg) == -1)
		cout << "Error: TLS Configuration failed" << endl;

	

	/* the message we send to the client */
	char buffer[1024];
	strlcpy(buffer, "It was the best of times, it was the worst of times... \n", sizeof(buffer));
	memset(&sockname, 0, sizeof(sockname));
	sockname.sin_family = AF_INET;
	sockname.sin_port = htons(port);
	sockname.sin_addr.s_addr = htonl(INADDR_ANY);

	
	/* Set up listening socket */
	struct sockaddr_in listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_socket == -1)
		cout << "Error: Create a socket" << endl;		
	if(bind(listen_socket, (struct sockaddr*)& sockname, sizeof(sockname)) == -1)
		cout << "Error: Bind failed" << endl;
	if(listen(sd, 3) == -1)
		cout << "Error: Listen failed" << endl;


	/* Set up constant listening*/
	while(true){

		int clientlen = sizeof(&client);
		int clientsd = accept(listen_socket, (struct sockaddr*) &client, &clientlen);
		if(clientsd == -1)
			cout << "Error: Accept failed" << endl;


		int pid = fork();
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
				}while(i == TLS_WANT_POLLIN || i == TLS_WANT_POLLOUT);
			}

			int w = 0;
			int written = 0;
			while(written < strlen(buffer)){
				w = tls_write(tls_cctx, buffer + written, strlen(buffer) - written);
				if(w == -1 && errno != EINTR){
					cout << "Error: TLS Write failed" << endl;
				}
				else{
					written += w;
				}
			}
			close(clientsd);
			exit(0);
		}
		close(clientsd);
	}
}

