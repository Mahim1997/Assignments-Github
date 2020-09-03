#include"FileReader.h"
#include "Router.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std ;


int main(int argc, char *argv[]){
	if(argc < 2){
		cout << "Not enough args... input IP Address, then topologyFile.txt" << endl;
		return -1;
	}
	string thisIpAddress = argv[1];
	string filename = argv[2];

    FileReader fileReader(filename);
    fileReader.readFile();




    Router router(thisIpAddress);
    router.setUpInitialRoutingTable(fileReader.sourceIps, fileReader.destIps, fileReader.costs, fileReader.setOfIPAddresses);
    router.showRouter();


	int sockfd; 
	int bind_flag;
	int bytes_received;
	socklen_t addrlen;
	char buffer[1024];
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(4747);
	server_address.sin_addr.s_addr = inet_addr(thisIpAddress.c_str());

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	bind_flag = bind(sockfd, (struct sockaddr*) &server_address, sizeof(sockaddr_in));
	router.setSocketID(sockfd);

	cout << "GOING TO RECEIVE NOW >....." << endl ;
	while(true){
		bytes_received = recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*) &client_address, &addrlen);
		router.processMessage(buffer, sockfd);
	}

}