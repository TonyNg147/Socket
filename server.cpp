#include <iostream>
#include <string>
#include <WS2tcpip.h>

#pragma comment(lib,"ws2_32.lib")

int main()
{
	WSADATA wsaData;
	int iResult;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		std::cerr << "Can not initialize winsock\n";
		return 1;
	}
	std::cout << "Initialize winsock successfully\n";
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		std::cerr << "Can not create a socket\n";
		WSACleanup();
		return 1;
	}
	std::cout << "Create asocket successfully\n";
	sockaddr_in hint;
	const char* ip = "127.0.0.1";
	constexpr int port = 54000;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ip, &hint.sin_addr);

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	listen(listening, 0);

	sockaddr_in clientaddr;
	int size = sizeof(clientaddr);
	SOCKET client;
	client = accept(listening, (sockaddr*)&clientaddr, &size);

	closesocket(listening);
	char host[NI_MAXHOST];
	char service[NI_MAXSERV];

	if (getnameinfo((sockaddr*)&clientaddr, size, host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		std::cout << host << " connected on port: " << service << std::endl;
	}
	else
	{
		inet_ntop(AF_INET, (sockaddr*)&clientaddr, host, NI_MAXHOST);
		std::cout << host << " connected on port: " << ntohs(hint.sin_port)<<std::endl;
	}

	char buff[4096];
	while (true)
	{
		iResult=recv(client, buff, 4096, 0);
		if (iResult < 0)
		{
			std::cout << "Error when receiving\n";
			break;
		}
		if (iResult == 0)
		{
			std::cout << "Client is quiting\n";
			break;
		}
		std::cout << "Received from client: " << std::string(buff, 0, iResult) << std::endl;
		send(client, buff, iResult + 1, 0);
	}
	closesocket(client);
	WSACleanup();

}