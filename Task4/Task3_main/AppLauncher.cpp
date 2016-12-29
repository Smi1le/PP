#include "stdafx.h"
#include <string>
#include <vector>
#include <iostream>
#include <Windows.h>
#include <chrono>

#include "../src/SocketHelper.h"
#include "../src/Helper.h"
#include "AppLauncher.h"


CAppLauncher::CAppLauncher(char * argv[])
	: m_itNumber(atoi(argv[2]))
	, m_procNumber(atoi(argv[1]))
{
	
}

void CAppLauncher::Run()
{
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
}

void CAppLauncher::WaitMessages()
{
	auto start = std::chrono::system_clock::now();
	std::vector<std::string> messages;
	
	std::cout << "Socket" << std::endl;
	CSocketHelper helper;
	helper.CreateSocket("192.168.0.2");
	helper.BindSocket();
	helper.WaitSend(messages, m_procNumber);
	
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> diff = end - start;
	std::cout << "Time : " << diff.count() << std::endl;
	for (auto const &mes : messages)
	{
		std::cout << mes << std::endl;
	}



}
