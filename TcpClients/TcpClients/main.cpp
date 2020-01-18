#include <iostream>
#include <string>
#include <WS2tcpip.h>


#pragma comment (lib , "ws2_32.lib")

using namespace std;

void main() {

	string ipAddress = "192.168.1.5";//ip address server-can use INADDR_ANY for localhost address
	int port = 54000;//Listening port number on the server

	//initialize winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsResult = WSAStartup(ver, &wsData);

	if (wsResult != 0) {
		cerr << "can't initialize winSock" << endl;
		return ;
	}

	//Create Socket
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET) {
		cerr << "Can't create socket" << endl;
		return ;
	}

	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &serverAddr.sin_addr);

	//Connect To Server
	int connectResult = connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));

	if (connectResult == SOCKET_ERROR) {
		cerr << "Can't Connect To Srever." << endl;
		closesocket(clientSocket);
		WSACleanup();
		return ;
	}

	//Do_While Loop For Send And Receive Data
	char buf[4096];
	string userInput;

	do {

		cout << "< ";
		getline(cin, userInput);

		if (userInput.size() > 0) {
			int sendByte = send(clientSocket, userInput.c_str(), userInput.size() + 1 , 0);

			if (sendByte != SOCKET_ERROR) {

				ZeroMemory(buf, 0);

				int receiveByte = recv(clientSocket, buf, 4096, 0);
				if (receiveByte > 0) {

					cout << "Server> " << string(buf, 0, receiveByte) << endl;

				}

			}

		}

	} while (userInput.size() > 0);

	//Close Socket
	closesocket(clientSocket);
	WSACleanup();

}