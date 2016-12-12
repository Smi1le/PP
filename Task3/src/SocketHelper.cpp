#include "stdafx.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <vector>
#include <string>
#include <iostream>
#include "SocketHelper.h"

#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "wsock32.lib")



bool CSocketHelper::CreateSocket()
{
	WSADATA ws;
	if (FAILED(WSAStartup(MAKEWORD(2, 2), &ws)))
	{
		std::cout << "Failed to initialize . ";
		std::cout << "Number error = " << WSAGetLastError() << std::endl;
		return false;
	}

	this->m_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (this->m_socket == INVALID_SOCKET)
	{
		std::cout << "Failed to create socket. ";
		std::cout << "Number error = " << WSAGetLastError() << std::endl;
		WSACleanup();
		return false;
	}
	
	return true;
}

bool CSocketHelper::TuneSocket(int port, ULONG address)
{

	SOCKADDR_IN sin;

	sin.sin_family = PF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = address;

	if (bind(this->m_socket, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		std::cout << "Unable to bind. Error number - " << WSAGetLastError() << std::endl;
		WSACleanup();
		return false;
	}

	return true;
}

bool CSocketHelper::TuneSocket(int port, std::string const & address)
{
	LPHOSTENT hostEnt;
	hostEnt = gethostbyname(address.c_str());

	if (!hostEnt)
	{
		std::cout << "Unable to collect gethostbyname. Error number = " << WSAGetLastError() << std::endl;
		return false;
	}

	SOCKADDR_IN info;

	info.sin_family = PF_INET;
	info.sin_addr = *((LPIN_ADDR)*hostEnt->h_addr_list);
	info.sin_port = htons(port);

	if (connect(this->m_socket, (LPSOCKADDR)&info, sizeof(info)) != 0)
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
	while (innerCount < messageNumber)
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

