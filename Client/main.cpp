#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <vector>
#include <string>
#pragma comment(lib, "Ws2_32.lib")
using namespace std;
//КЛИЕНТ

int main(void) {
	WSADATA wsData;
	int erStat = WSAStartup(MAKEWORD(2, 2), &wsData);//вызов функции запуска оскетов

	if (erStat != 0) {
		cout << "Error WinSock version initializaion #";
		cout << WSAGetLastError();
		return 1;
	}
	else
		cout << "WinSock initialization is OK" << endl;
	SOCKET ClientSock = socket(AF_INET, SOCK_STREAM, 0);
	if (ClientSock == INVALID_SOCKET) {
		cout << "Error initialization socket # " << WSAGetLastError() << endl;
		closesocket(ClientSock);
		WSACleanup();
		return 1;
	}
	else
		cout << "Client socket initialization is OK" << endl;
	//ИНИЦИАЛИЗАЦИЯ ПОДКЛЮЧЕНИЯ

	in_addr ip_to_num;
	const int buff_size = 100;//буфефр для ввода  IP
	char ip_input[buff_size]{};
	cout << "Connection port: 2004" << endl;
	cout << "Enter IP you want to connect" << endl;
	cin.getline(ip_input, buff_size);
	

	erStat = inet_pton(AF_INET, ip_input, &ip_to_num);
	if (erStat <= 0) {
		cout << "Error in IP translation to special numeric format" << endl;
		return 1;
	}
	sockaddr_in servInfo;

	ZeroMemory(&servInfo, sizeof(servInfo));

	servInfo.sin_family = AF_INET;
	servInfo.sin_addr = ip_to_num;	  // Server's IPv4 after inet_pton() function
	servInfo.sin_port = htons(2004);

	erStat = connect(ClientSock, (sockaddr*)&servInfo, sizeof(servInfo));

	if (erStat != 0) {
		cout << "Connection to Server is FAILED. Error # " << WSAGetLastError() << endl;
		closesocket(ClientSock);
		WSACleanup();
		return 1;
	}
	else
		cout << "Connection established SUCCESSFULLY. Ready to send a message to Server"
		<< endl;
	//ОТПРАВКА ДАННЫХ
	vector<char> clientBuff(256),servBuff(256);
	while (true) {
		cout << "Enter data or 's' for shutting down" << endl;
		fgets(clientBuff.data(), clientBuff.size(), stdin);
		auto data_send_s = send(ClientSock, clientBuff.data(), clientBuff.size(), 0);
		if (data_send_s == SOCKET_ERROR) {
			cout << "Error while sending data" << endl;
			closesocket(ClientSock);
			WSACleanup();
			return 0;
		}
		if (clientBuff[0] == 's') {
			cout << "server shutted down" << endl;
			closesocket(ClientSock);
			WSACleanup();
			return 0;
		}
		auto data_rec_s = recv(ClientSock, servBuff.data(), servBuff.size(), 0);
		if (data_rec_s == SOCKET_ERROR) {
			cout << "Error while recieving response from server " << endl;
			closesocket(ClientSock);
			WSACleanup();
			return 0;
		}
		else {
			cout << "Server response - generated name: " << endl;
			string received = servBuff.data();
			cout << received << endl;
		}
		
		
		
		
	}
}




