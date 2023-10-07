#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <string>


#pragma comment(lib, "Ws2_32.lib")

//СЕРВЕР
using namespace std;

int main(void) {

	srand(time(0));
	//инициализация версий сокетов и интерфейсов
	WSADATA wsData; //определяет версию сокетов для нашей ос
	int erStat = WSAStartup(MAKEWORD(2, 2), &wsData);//вызов функции запуска оскетов

	if (erStat != 0) {
		cout << "Error WinSock version initializaion #";
		cout << WSAGetLastError();
		return 1;
	}
	else
		cout << "WinSock initialization is OK" << endl;

	//создание сокета и его инициализация 
	SOCKET ServSock = socket(AF_INET, SOCK_STREAM, 0);

	if (ServSock == INVALID_SOCKET) {
		cout << "Error initialization socket # " << WSAGetLastError() << endl;
		closesocket(ServSock);
		WSACleanup();
		return 1;
	}
	else
		cout << "Server socket initialization is OK" << endl;
	//привязка сокета к паре IP/Port
	in_addr ip_to_num;
	erStat = inet_pton(AF_INET, "25.9.57.79", &ip_to_num);
	if (erStat <= 0) {
		cout << "Error in IP translation to special numeric format" << endl;
		return 1;
	}

	sockaddr_in servInfo;
	ZeroMemory(&servInfo, sizeof(servInfo));

	servInfo.sin_family = AF_INET;
	servInfo.sin_addr = ip_to_num;
	servInfo.sin_port = htons(2004);

	erStat = bind(ServSock, (sockaddr*)&servInfo, sizeof(servInfo));
	if (erStat != 0) {
		cout << "Error Socket binding to server info. Error # " << WSAGetLastError() << endl;
		closesocket(ServSock);
		WSACleanup();
		return 1;
	}
	else
		cout << "Binding socket to Server info is OK" << endl;
	cout << "Server started at IP: " << inet_ntoa(servInfo.sin_addr) << " Port: " << htons(servInfo.sin_port) << endl;
	//прослушка привязанного порта
	erStat = listen(ServSock, SOMAXCONN);

	if (erStat != 0) {
		cout << "Can't start to listen to. Error # " << WSAGetLastError() << endl;
		closesocket(ServSock);
		WSACleanup();
		return 1;
	}
	else {
		cout << "Listening..." << endl;
	}
	sockaddr_in clientInfo;

	ZeroMemory(&clientInfo, sizeof(clientInfo));

	int clientInfo_size = sizeof(clientInfo);

	SOCKET ClientConn = accept(ServSock, (sockaddr*)&clientInfo, &clientInfo_size);

	if (ClientConn == INVALID_SOCKET) {
		cout << "Client detected, but can't connect to a client. Error # " << WSAGetLastError() << endl;
		closesocket(ServSock);
		closesocket(ClientConn);
		WSACleanup();
		return 1;
	}
	else
		cout << "Connection to a client established successfully" << endl;
	//получение данных
	vector <char> servBuff(256);
	while (true) {
		auto data_rec_s = recv(ClientConn, servBuff.data(), servBuff.size(), 0);

		if (data_rec_s == SOCKET_ERROR) {
			cout << "Error while recieving data" << endl;
			closesocket(ServSock);
			closesocket(ClientConn);
			WSACleanup();
			return 0;
		}
		if (servBuff[0] == 's') {
			cout << "Server shutted down" << endl;
			closesocket(ServSock);
			closesocket(ClientConn);
			WSACleanup();
			return 0;
		}
		else {
			string recieved = servBuff.data();
			cout << "Received data: " << recieved << endl;
			int file_number = rand();
			string file_name = to_string(file_number) + ".txt";
			ofstream file(file_name);
			file << recieved;
			file.close();
			

			//file.close();
			vector<char> clientBuff(file_name.begin(), file_name.end());
			auto data_resp_s = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);


		}
	}
}
