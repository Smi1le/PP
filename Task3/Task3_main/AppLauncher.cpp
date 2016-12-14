#include "stdafx.h"
#include <string>
#include <vector>
#include <iostream>
#include <Windows.h>
#include "../src/SocketHelper.h"
#include "../src/Helper.h"
#include "../src/PipeHelper.h"
#include "AppLauncher.h"


CAppLauncher::CAppLauncher(char * argv[], DataTransferType type)
	: m_itNumber(atoi(argv[2]))
	, m_procNumber(atoi(argv[1]))
	, m_type(type)
{
}

void CAppLauncher::Run()
{
	int failedProc = 0;
	for (size_t i = 0; i != m_procNumber; ++i)
	{
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		ZeroMemory(&si, sizeof(si));
		std::string commandLine = "Task3.exe " + helper::NumberToString(m_itNumber) + " " + helper::NumberToString(i + 1);

		if (m_type == DataTransferType::Socket)
			commandLine += " socket";
		else
			commandLine += " pipe";

		SettingProcess(si);

		if (!CreateProcess(NULL, (LPSTR)commandLine.data(), NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
		{
			++failedProc;
			std::cout << "Could't create a process. Program will continue to work without it" << std::endl;
			continue;
		}


		m_startUpInfos.push_back(si);
		m_processesInformation.push_back(pi);
	}
	m_procNumber -= failedProc;

	WaitMessages();
}

void CAppLauncher::SettingProcess(STARTUPINFO &si)
{
	si.dwFlags = STARTF_USESIZE | STARTF_USESHOWWINDOW;
	si.dwXSize = 500;
	si.dwYSize = 300;
	si.wShowWindow = SW_SHOW;
}

CAppLauncher::~CAppLauncher()
{
	for (size_t i = 0; i != m_procNumber; ++i)
	{
		CloseHandle(m_processesInformation[i].hProcess);
		CloseHandle(m_processesInformation[i].hThread);
	}
}

void CAppLauncher::WaitMessages()
{
	std::vector<std::string> messages;
	if (m_type == DataTransferType::Socket)
	{
		CSocketHelper helper;
		helper.CreateSocket();
		helper.TuneSocket(11111, INADDR_ANY);
		helper.WaitSend(messages, m_procNumber);
	}
	else
	{
		SPipeHelper::WaitSend(messages, m_procNumber);
	}
	for (auto const &mes : messages)
	{
		std::cout << mes << std::endl;
	}

}
