#include "stdafx.h"
#include <windows.h>
#include <vector>
#include <string>
#include <iostream>
#include "PipeHelper.h"

void SPipeHelper::SendMes(std::string const & message)
{
	HANDLE hPipe;

	do {
		hPipe = CreateFile(TEXT("\\\\.\\pipe\\myPipe"),
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);
	} while (hPipe == INVALID_HANDLE_VALUE);

	DWORD dwWritten = static_cast<DWORD>(message.size());
	if (hPipe != INVALID_HANDLE_VALUE)
	{
		while (WriteFile(hPipe,
			message.data(),
			static_cast<DWORD>(message.size()),   // = length of string + terminating '\0' !!!
			&dwWritten,
			NULL) == FALSE)
		{
		}

		CloseHandle(hPipe);
	}
}

void SPipeHelper::WaitSend(std::vector<std::string>& messages, size_t messageNumber)
{
	HANDLE hPipe = CreateNamedPipe("\\\\.\\pipe\\myPipe",
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES,
		1024, 1024, 5000, NULL);

	if (hPipe == INVALID_HANDLE_VALUE)
	{
		std::cout << "Failed" << std::endl;
		return;
	}

	size_t innerCount = 0;
	while (innerCount < messageNumber)
	{
		if (ConnectNamedPipe(hPipe, NULL) != FALSE)
		{
			char buffer[1024];
			DWORD dwRead;
			if (ReadFile(hPipe, buffer, sizeof(buffer), &dwRead, NULL) == TRUE)
			{
				buffer[dwRead] = '\0';
			}
			messages.push_back(buffer);
			++innerCount;

		}
		DisconnectNamedPipe(hPipe);
	}
	CloseHandle(hPipe);
}

