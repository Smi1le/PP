#pragma once
#include <iostream>
#include <vector>
#include "CBankClient.h"
class CBankClient;

class CBank
{
public:
	CBank();
	~CBank();
	CBankClient* CreateClient();
	void UpdateClientBalance(CBankClient& client, int value);
	size_t GetNumberClients() const;
	HANDLE* GetHandles() const;
private:
	std::vector<CBankClient> m_clients;
	int m_totalBalance;
	HANDLE m_hMutex;

	int GetTotalBalance();
	void SetTotalBalance(int value);
	void SomeLongOperations();
};