#include "stdafx.h"
#include <iostream>
//#include <windows.h>
#include <vector>
#include <string>
#include <sstream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include "../src/SocketHelper.h"
#include "../src/Helper.h"
#include "../src/PipeHelper.h"

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
	void WaitMessages(SInfoRunProcesses &info, std::string const &type)
	{
		std::vector<std::string> messages;
		if (type != "pipe")
		{
			CSocketHelper helper;
			helper.CreateSocket();
			helper.TuneSocket(11111, INADDR_ANY);
			helper.WaitSend(messages, info.processesNumber);
		}
		else
		{
			SPipeHelper::WaitSend(messages, info.processesNumber);
		}
		for (auto const &mes : messages)
		{
			std::cout << mes << std::endl;
		}
		
	}



	std::string GetName(std::string firstName, size_t number, std::string const &namePipe)
	{
		std::string name = firstName + helper::NumberToString(number) + namePipe;
		return name;
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
			std::string commandLine = GetName("Task3.exe ", info.iterationsNumber, " " + helper::NumberToString(i + 1));
			
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
	void Start(char *argv[])
	{
		SInfoRunProcesses info;
		info.processesNumber = atoi(argv[1]);
		info.iterationsNumber = atoi(argv[2]);
		RunProcess(info);
		
		WaitMessages(info, "pipe");
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

