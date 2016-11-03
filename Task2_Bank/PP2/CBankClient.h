#pragma once
#include "CBank.h"
class CBank;

class CBankClient
{
public:
	unsigned int GetId();
	~CBankClient() = default;
private:
	unsigned m_id;
	CBank *m_bank;

	CBankClient(CBank *bank, unsigned int id, SyncPrimitives *syncPrimitives);
	
	static unsigned int GetSleepDuration(CBankClient *client);
	static unsigned int GetBalanceChangeValue();
	static DWORD WINAPI ThreadFunction(LPVOID lpParam);

	friend CBank;

private:
	HANDLE m_handle;
	SyncPrimitives *m_syncPrimitives = nullptr;
};