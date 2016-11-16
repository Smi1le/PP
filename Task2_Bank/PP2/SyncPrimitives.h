#pragma once
#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <vector>
#include <Windows.h>

enum class TypeSyncPrimitives
{
	CRITICAL_SECTION,
	MUTEX,
	SEMAPHORE,
	EVENT,
	NOT
};


struct SyncPrimitives
{
	TypeSyncPrimitives type;
	HANDLE hMutex;
	HANDLE hSemaphore;
	HANDLE hEvent;
	CRITICAL_SECTION critical_section;


	SyncPrimitives()
		: hMutex(CreateMutex(NULL, false, NULL))
		, hSemaphore(CreateSemaphore(NULL, 1, 1, NULL))
		, hEvent(CreateEvent(NULL, false, true, NULL))
	{
		InitializeCriticalSection(&critical_section);
	}
};