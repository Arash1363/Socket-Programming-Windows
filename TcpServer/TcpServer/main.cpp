#include <iostream>
#include <WS2tcpip.h>

#pragma comment (lib , "ws2_32.lib") //link with ws2_32.lib library file

using namespace std;

void main() {
	//initialize winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int winSock = WSAStartup(ver, &wsData);
	if (winSock != 0) {
		cerr << "winsock can't initialize" << endl;
			return;
	}
	
	//Create Socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET) {
		cerr << "can't create socket." << endl ;
		return;
	}

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(54000);
	address.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	//Bind the ip and port to a socket
	if (bind(listening, (sockaddr*)&address, sizeof(address)) == -1) {
		cerr << "can not binding" << endl;
		return;
	}

	//Listen For Connection
	listen(listening, 5);

	//Wait For A Connection
	sockaddr_in clientAddr;
	socklen_t clientSize = sizeof(clientAddr);

	SOCKET clientSocket = accept(listening, (sockaddr*)&clientAddr, &clientSize);

	if (clientSocket == INVALID_SOCKET) {
		cerr << "Can't create socket by accepy method." << endl;
		return;
	}

	char host[4096];//Client remote name
	char service[4096];//Service the client connected on

	ZeroMemory(host, 4096);
							//Same as memset(host , 0 , 4096)	
	ZeroMemory(service, 4096);

	if (getnameinfo((sockaddr*)&clientAddr, clientSize, host, 4096, service, 4096, 0) == 0) {
		cout << host << " connected on " << service << endl;
	}
	else {
		inet_ntop(AF_INET, &clientAddr.sin_addr, host, 4096);
		cout << host << " connected on port " << ntohs(clientAddr.sin_port) << endl;
	}

	//Close Socket Listening
	closesocket(listening);

	//While loop for display message
	char buf[4096];

	while (true) {
		ZeroMemory(buf, 0);

		//Receive client data
		int receiveByte = recv(clientSocket, buf, 4096, 0);

		if (receiveByte == SOCKET_ERROR) {
			cerr << "Error in receive , Exit." << endl;
			break;
		}

		if (receiveByte == 0) {
			cerr << "Cleint disconnected." << endl;
			break;
		}

		//Send back data to cleint
		send(clientSocket, buf, receiveByte + 1, 0);

	}
	//Close Socket
	closesocket(clientSocket);

	//Cleanup winSock
	WSACleanup();

	system("pause");//Pause Console
	

}