#pragma once
#include "CBankClient.h"
class CBankClient;

class CBank
{
public:
	CBank(TypeSyncPrimitives type);
	~CBank();
	CBankClient* CreateClient();
	void UpdateClientBalance(CBankClient& client, int value);
	size_t GetClientsCount() const;
	HANDLE* GetClientsHandles() const;
private:
	std::vector<CBankClient> m_clients;
	int m_totalBalance;
	//HANDLE m_hMutex;
	//CRITICAL_SECTION m_cr;
	//TypeSyncPrimitives m_typePrimitiveSync;
	SyncPrimitives *m_syncPrimitives = nullptr;

	int GetTotalBalance();
	void SetTotalBalance(int value);
	void SomeLongOperations(CBankClient const &client);

};