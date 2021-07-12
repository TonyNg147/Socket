#include <iostream>
#include <WS2tcpip.h>
#include <string>

#pragma comment(lib,"ws2_32.lib")

int main()
{
	WSADATA wsaData;
	int iResult;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		std::cerr << "Cannot initialize winsock\n";
		return 1;
	}
	std::cout << "Initialize winsock successfully\n";
	SOCKET client = socket(AF_INET, SOCK_STREAM, 0);
	if (client == INVALID_SOCKET)
	{
		std::cerr << "Cannot create a socket\n";
		WSACleanup();
		return 1;
	}
	std::cout << "Create a socket successfully\n";
	sockaddr_in hint;
	const char* ip = "127.0.0.1";
	constexpr int port = 54000;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ip, &hint.sin_addr);

	iResult = connect(client, (sockaddr*)&hint, sizeof(hint));
	if (iResult == SOCKET_ERROR)
	{
		std::cerr << "Cannot connect to server\n";
		closesocket(client);
		WSACleanup();
		return 1;
	}
	std::cout << "Connect to server successfully\n";

	char buff[4096];
	std::string in;
	do
	{
		std::cout << "Send to server: ";std::getline(std::cin, in);
		if (in.size() > 0)
		{
			iResult = send(client, in.c_str(), in.size(), 0);
			if (iResult > 0)
			{
				ZeroMemory(buff, 4096);
				iResult = recv(client, buff, 4096, 0);
				if (iResult > 0)
				{
					std::cout << "Received from server: " << std::string(buff, 0, iResult) << std::endl;
				}
			}
		}
	} while (in.size() > 0);
	closesocket(client);
	WSACleanup();
}