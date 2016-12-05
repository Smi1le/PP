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
			std::string command = "Task3.exe " + IntToString(info.iterationsNumber);
			std::vector<TCHAR> czCommandLine;
			czCommandLine.resize(command.size());
			std::copy(command.begin(), command.end(), czCommandLine.begin());
			SettingProcess(si);
			
			if (!CreateProcess(NULL, LPWSTR(czCommandLine.data()), NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
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

	void Start(char *argv[])
	{
		SInfoRunProcesses info;
		info.processesNumber = atoi(argv[1]);
		info.iterationsNumber = atoi(argv[2]);
		RunProcess(info);
		WaitForMultipleObjects(3, info.handles.data(), TRUE, INFINITE);
		CloseProcesses(info);
	}
}


int main(int argc, char *argv[])
{
	if (argc != 3)
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

