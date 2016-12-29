#include "stdafx.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <vector>
#include <string>
#include <iostream>
#include "SocketHelper.h"

#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "wsock32.lib")



bool CSocketHelper::CreateSocket(std::string const & address)
{
	WSADATA ws;
	if (FAILED(WSAStartup(MAKEWORD(2, 2), &ws)))
	{
		std::cout << "Failed to initialize . ";
		std::cout << "Number error = " << WSAGetLastError() << std::endl;
		return false;
	}

	struct addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));

	// AF_INET определ€ет, что используетс€ сеть дл€ работы с сокетом
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM; // «адаем потоковый тип сокета
	hints.ai_protocol = IPPROTO_TCP; // »спользуем протокол TCP
									 // —окет биндитс€ на адрес, чтобы принимать вход€щие соединени€
	hints.ai_flags = AI_PASSIVE;

	int result = getaddrinfo(address.c_str(), "11111", &hints, &m_addr);

	// ≈сли инициализаци€ структуры адреса завершилась с ошибкой,
	// выведем сообщением об этом и завершим выполнение программы 
	if (result != 0) {
		std::cerr << "getaddrinfo failed: " << result << "\n";
		WSACleanup(); // выгрузка библиотеки Ws2_32.dll
		return 1;
	}

	this->m_socket = socket(m_addr->ai_family, m_addr->ai_socktype, m_addr->ai_protocol);

	if (this->m_socket == INVALID_SOCKET)
	{
		std::cout << "Failed to create socket. ";
		std::cout << "Number error = " << WSAGetLastError() << std::endl;
		WSACleanup();
		return false;
	}
	
	return true;
}

bool CSocketHelper::BindSocket()
{
	if (bind(this->m_socket, m_addr->ai_addr, (int)m_addr->ai_addrlen) == SOCKET_ERROR)
	{
		std::cout << "Unable to bind. Error number - " << WSAGetLastError() << std::endl;
		WSACleanup();
		return false;
	}

	return true;
}

bool CSocketHelper::ConnectSocket()
{
	if (connect(this->m_socket, m_addr->ai_addr, m_addr->ai_addrlen) != 0)
	{
		std::cout << "Unable to connect. Error number = " << WSAGetLastError() << std::endl;
		WSACleanup();
		return false;
	}



	return true;
}

bool CSocketHelper::SendMes(std::string const & message)
{

	if (send(this->m_socket, message.c_str(), static_cast<int>(message.size()), 0) == SOCKET_ERROR)
	{
		std::cout << "Unable to send. Error number = " << WSAGetLastError() << std::endl;
		WSACleanup();
		return false;
	}
	return true;
}

bool CSocketHelper::WaitSend(std::vector<std::string>& messages, size_t messageNumber)
{
	size_t innerCount = 0;
	while (true)
	{
		if (listen(this->m_socket, 10) == SOCKET_ERROR)
		{
			std::cout << "Unable to listen" << std::endl;
			WSACleanup();
			return false;
		}
		SOCKADDR_IN from;
		int fromlen = sizeof(from);
		SOCKET clientSock = accept(this->m_socket, (struct sockaddr*)&from, &fromlen);

		if (clientSock < 0)
		{
			closesocket(clientSock);
			continue;
		}

		char RecvBuffer[1];
		std::string send;
		int iResult;
		do {
			iResult = recv(clientSock, RecvBuffer, sizeof(RecvBuffer), 0);
			if (iResult > 0)
			{
				send += RecvBuffer[0];
			}
			else if (iResult == 0)
			{
				printf("Connection closed\n");
			}
			else
			{
				std::cout << "recv failed: " << WSAGetLastError() << std::endl;
				WSACleanup();
				return false;
			}

		} while (iResult > 0);
		std::cout << "Send = " << send << std::endl;
		messages.push_back(send);
		++innerCount;
		closesocket(clientSock);
	}
	return true;
}

CSocketHelper::~CSocketHelper()
{
	closesocket(this->m_socket);
}

