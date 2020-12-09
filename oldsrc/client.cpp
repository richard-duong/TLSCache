// Standard Libraries
#include <iostream>

// External Libraries
#include <tls.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

using namespace std;

int main(){

	/* Initialize */
	struct sockaddr_in server_sa;
	struct tls_config* tls_cfg = NULL;
	int port = 9999;	
	int out_socket = -1;
			

	/* Setup TLS */
	if(tls_init() == -1)
		cout << "Error: Unable to initialize TLS" << endl;

	if ((tls_cfg = tls_config_new()) == NULL)
		cout << "Error: Unable to allocate TLS config" << endl;

	if (tls_config_set_ca_file(tls_cfg, "../certs/root.pem") == -1)
		cout << "Error: Unable to set root CA file" << endl;


	/* Setup server_sa */
	memset(&server_sa, 0, sizeof(server_sa));
	server_sa.sin_family = AF_INET;
	server_sa.sin_port = htons(port);
	if(server_sa.sin_addr.s_addr == INADDR_NONE){
		cout << "Error: Invalid IP address " << argv[1] << endl;
		usage();
	}

	/* Get socket */
	int out_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(out_socket == -1)
		cout << "Error: Socket failed" << endl;


	/* Connect to server */
	if(connect(out_socket, (struct sockaddr *)& server_sa, sizeof(server_sa)) == -1)
		cout << "Error: Connect failed" << endl;

	
	/* Check for tls creation */
	struct* tls tls_ctx;
	if((tls_ctx = tls_client()) == NULL)
		cout << "Error: Create Client failed" << endl;

	/* Configuration for client */
	if(tls_configure(tls_ctx, tls_cfg) == -1)
		cout << "Error: Failed to configure client" << endl;

	/* Connect socket with tls before establishing handshake*/
	if(tls_connect_socket(tls_ctx, out_socket, "localhost") == -1)
		cout << "Error: Failed TLS Connection" << endl;


	/* Establish TLS handshake */	
	int i;	
	do{
		if((i = tls_handshake(tls_ctx)) == -1)
			cout << "Error: TLS Handshake failed" << endl;
	}while(i == TLS_WANT_POLLIN || i == TLS_WANT_POLLOUT);
			

	char buffer[1024];
	int r = -1;
	int rc = 0;
	int maxread = sizeof(buffer) - 1;
	while((r != 0) && rc < maxread){
		r = tls_read(tls_ctx, buffer + rc, maxread - rc);
		if(r == TLS_WANT_POLLIN || r == TLS_WANT_POLLOUT)
			continue;
		if(r < 0)
			cout << "Error: TLS read failed" << endl;
		else
			rc += r;			
	}

	buffer[rc] = '\0';
	cout << "Server sent: buffer" << endl;
	close(out_socket);
	return 0;
}
