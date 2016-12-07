#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <sstream>


using namespace std;

struct SInfoRunProcesses
{
	std::vector<STARTUPINFO> startUpInfos;
	std::vector<PROCESS_INFORMATION> processInformations;
	std::vector<bool> listFinishedProcesses;
	HANDLE hPipe;
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

	std::string GetName(std::string firstName, size_t number, std::string const &namePipe)
	{
		std::string name = firstName + IntToString(number) + namePipe;
		std::vector<TCHAR> allName;
		allName.resize(name.size());
		//std::cout << allName.size() << std::endl;
		std::copy(name.begin(), name.end(), allName.begin());
		//TCHAR f[] = { *allName.data() };
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
		CreatePipeForProcesses(info);
		RunProcess(info);
		
		WaitForMultipleProcesses(info);
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

