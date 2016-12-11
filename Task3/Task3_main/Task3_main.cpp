#include "stdafx.h"
#include <iostream>
//#include <windows.h>
#include <vector>
#include <string>
#include <sstream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "wsock32.lib")


using namespace std;

struct SInfoRunProcesses
{
	std::vector<STARTUPINFO> startUpInfos;
	std::vector<PROCESS_INFORMATION> processInformations;
	std::vector<bool> listFinishedProcesses;
	HANDLE hPipe;
	SOCKET serverSock;
	std::vector<HANDLE> handles;
	size_t processesNumber;
	size_t iterationsNumber;
};

namespace
{
	std::string IntToString(size_t number)
	{
		std::ostringstream os;
		os << number;
		return os.str();

	}

	void CreateSockets(SInfoRunProcesses &info)
	{
		/*WSADATA ws;
		if (FAILED(WSAStartup(MAKEWORD(2, 2), &ws)))
		{
			auto error = WSAGetLastError();
		}

		info.serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (info.serverSock == INVALID_SOCKET)
		{
			auto error = WSAGetLastError();
			std::cout << "error1 = " << error << std::endl;
		}
		SOCKADDR_IN sin;

		sin.sin_family = AF_INET;
		sin.sin_port = htons(11111);

		LPHOSTENT hostEnt;
		hostEnt = gethostbyname("localhost");
		sin.sin_addr = *((LPIN_ADDR)hostEnt->h_addr_list);
		//sin.sin_addr.s_addr = INADDR_ANY;


		int retVal = bind(info.serverSock, (LPSOCKADDR)&sin, sizeof(sin));

		if (retVal == SOCKET_ERROR)
		{
			auto error = WSAGetLastError();
			std::cout << "error2 = " << error << std::endl;
		}*/
		/*Часть где мы связываемся с клиентом. Перенести в другой метод*/
		/*SOCKET clientSock;
		SOCKADDR_IN from;
		int fromlen = sizeof(from);
		clientSock = accept(sock, (struct sockaddr*)&from, &fromlen);*/


	}

	void WaitForMultipleProcessesToSockets(SInfoRunProcesses &info)
	{
		WSADATA ws;
		if (FAILED(WSAStartup(MAKEWORD(2, 2), &ws)))
		{
			auto error = WSAGetLastError();
		}

		info.serverSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (info.serverSock == INVALID_SOCKET)
		{
			auto error = WSAGetLastError();
			std::cout << "error1 = " << error << std::endl;
		}
		SOCKADDR_IN sin;

		sin.sin_family = PF_INET;
		sin.sin_port = htons(11111);

		sin.sin_addr.s_addr = INADDR_ANY;


		int retVal = bind(info.serverSock, (LPSOCKADDR)&sin, sizeof(sin));

		if (retVal == SOCKET_ERROR)
		{
			auto error = WSAGetLastError();
			std::cout << "error2 = " << error << std::endl;
		}

		size_t countFinishedProcesses = 0;
		while (countFinishedProcesses < info.processesNumber)
		{

			
			if (listen(info.serverSock, 10) == SOCKET_ERROR)
			{
				printf("Unable to listen\n");
				WSACleanup();
			}

			std::cout << "first" << std::endl;
			SOCKET clientSock;
			SOCKADDR_IN from;
			int fromlen = sizeof(from);

			clientSock = accept(info.serverSock, (struct sockaddr*)&from, &fromlen);
			
			cout << "second" << endl;

			if (clientSock < 0)
			{
				std::cout << "Go fast" << std::endl;
				closesocket(clientSock);
				continue;
			}
			//printf("%s\n", from.sin_addr);
			//std::cout << int(from.sin_addr) << std::endl;
			std::cout << from.sin_family << std::endl;
			std::cout << from.sin_port << std::endl;

			cout << "third" << endl;

			char RecvBuffer[1];
			std::string send;
			/*int returnValue = recv(clientSock, RecvBuffer, sizeof(RecvBuffer), 0);
			while (returnValue != SOCKET_ERROR)
			{
				std::cout << "return Value = " << returnValue << std::endl;
				printf("%c", RecvBuffer[0]);
				send += RecvBuffer[0];
				returnValue = recv(clientSock, RecvBuffer, sizeof(RecvBuffer), 0);

				//send(s1, MsgText, sizeof(MsgText), MSG_DONTROUTE);
				system("pause");
			}*/
			int iResult;
			do {

				iResult = recv(clientSock, RecvBuffer, sizeof(RecvBuffer), 0);
				if (iResult > 0)
				{
					send += RecvBuffer[0];
					printf("Bytes received: %d\n", iResult);
				}
				else if (iResult == 0)
				{
					printf("Connection closed\n");
				}
				else
				{
					printf("recv failed: %d\n", WSAGetLastError());
				}

			} while (iResult > 0);

			cout << "fourth" << endl;

			if (send.size() > 0)
			{
				std::cout << "send = " << send << std::endl;
			}
			++countFinishedProcesses;
			closesocket(clientSock);
		}
	}



	std::string GetName(std::string firstName, size_t number, std::string const &namePipe)
	{
		std::string name = firstName + IntToString(number) + namePipe;
		return name;
	}

	bool CreatePipeForProcesses(SInfoRunProcesses &info)
	{
		//
		
		
		info.hPipe = CreateNamedPipe("\\\\.\\pipe\\myPipe",
			PIPE_ACCESS_DUPLEX, 
			PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, 
			PIPE_UNLIMITED_INSTANCES,
			1024, 1024, 5000, NULL);

		if (info.hPipe == INVALID_HANDLE_VALUE)
		{
			std::cout << "*-*-*-*-*-*-*-*-*" << std::endl;
			return false;
		}

		return true;
	}

	

	

	void SettingProcess(STARTUPINFO &si)
	{
		si.dwFlags = STARTF_USESIZE | STARTF_USESHOWWINDOW;
		si.dwXSize = 500;
		si.dwYSize = 300;
		si.wShowWindow = SW_SHOW;
	}

	void RunProcess(SInfoRunProcesses &info)
	{
		for (size_t i = 0; i != info.processesNumber; ++i)
		{
			STARTUPINFO si;
			PROCESS_INFORMATION pi;
			ZeroMemory(&si, sizeof(si));
			std::string commandLine = GetName("Task3.exe ", info.iterationsNumber, " " + IntToString(i + 1));
			
			SettingProcess(si);
			
			if (!CreateProcess(NULL, (LPSTR)commandLine.data(), NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
			{
				std::cout << "Could't create a process. Program will continue to work without it" << std::endl;
				continue;
			}

			
			info.startUpInfos.push_back(si);
			info.processInformations.push_back(pi);
			info.handles.push_back(pi.hThread);
		}

		

	}

	void CloseProcesses(SInfoRunProcesses &info)
	{
		for (size_t i = 0; i != info.processesNumber; ++i)
		{
			CloseHandle(info.processInformations[i].hProcess);
			CloseHandle(info.processInformations[i].hThread);
		}
		closesocket(info.serverSock);
		WSACleanup();
	}

	void WaitForMultipleProcesses(SInfoRunProcesses &info)
	{
		size_t countFinishedProcesses = 0;
		while (countFinishedProcesses < info.processesNumber)
		{
			if (ConnectNamedPipe(info.hPipe, NULL) != FALSE)
			{
				std::cout << "Yes" << std::endl;
				char buffer[1024];
				DWORD dwRead;
				if (ReadFile(info.hPipe, buffer, sizeof(buffer), &dwRead, NULL) == TRUE)
				{
					/* add terminating zero */
					std::cout << "dwRead = " << dwRead << std::endl;
					buffer[dwRead] = '\0';

					std::cout << "buffer = " << buffer << std::endl;
					for (size_t i = 0; i != dwRead; ++i)
					{
						std::cout << buffer[i] << "  --  " << static_cast<int>(buffer[i]) << std::endl;
					}
					std::cout << std::endl;
					/* do something with data in buffer */
					printf("%s", buffer);
				}
				std::cout << "dwRead = " << dwRead << std::endl;
				std::cout << "buffer = " << buffer << std::endl;
				++countFinishedProcesses;
				
			}
			DisconnectNamedPipe(info.hPipe);
		}
	}

	void Start(char *argv[])
	{
		SInfoRunProcesses info;
		info.processesNumber = atoi(argv[1]);
		info.iterationsNumber = atoi(argv[2]);
		//CreatePipeForProcesses(info);
		CreateSockets(info);
		RunProcess(info);
		
		//WaitForMultipleProcesses(info);
		WaitForMultipleProcessesToSockets(info);
		//WaitForMultipleObjects(DWORD(info.processesNumber), info.handles.data(), TRUE, INFINITE);
		CloseProcesses(info);
	}
}


int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		std::cerr << "Error! Usage program.exe <processes number> <iterations number>" << std::endl;
		return EXIT_FAILURE;
	}

	try
	{
		Start(argv);
		return 0;
	}
	catch(std::exception &ex)
	{
		std::cerr << ex.what() << std::endl;
	}
}

