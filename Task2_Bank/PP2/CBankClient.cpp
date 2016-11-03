#include "stdafx.h"
#include "CBankClient.h"

CBankClient::CBankClient(CBank *bank, unsigned int id, SyncPrimitives *primitives)
	: m_syncPrimitives(primitives)
	, m_bank(bank)
	, m_id(id)
	, m_handle(CreateThread(NULL, 0, ThreadFunction, this, 0, NULL))
{}

unsigned int CBankClient::GetId()
{
	return m_id;
}

void SomeLongOperation(unsigned time)
{
	Sleep(time);
}

DWORD WINAPI CBankClient::ThreadFunction(LPVOID lpParam)
{
	
	CBankClient *client = (CBankClient*)lpParam;
	
	srand(client->m_id);
	
	while (true)
	{
		SomeLongOperation(GetSleepDuration(client));
		
		switch (client->m_syncPrimitives->type)
		{
			case TypeSyncPrimitives::CRITICAL_SECTION:
				EnterCriticalSection(&client->m_syncPrimitives->critical_section);
				client->m_bank->UpdateClientBalance(*client, GetBalanceChangeValue());
				LeaveCriticalSection(&client->m_syncPrimitives->critical_section);
				break;
			case TypeSyncPrimitives::SEMAPHORE:
				WaitForSingleObject(client->m_syncPrimitives->semaphore, INFINITE);
				client->m_bank->UpdateClientBalance(*client, GetBalanceChangeValue());
				ReleaseSemaphore(client->m_syncPrimitives->semaphore, 1, NULL);
				break;
			case TypeSyncPrimitives::MUTEX:
				WaitForSingleObject(client->m_syncPrimitives->mutex, INFINITE);
				client->m_bank->UpdateClientBalance(*client, GetBalanceChangeValue());
				ReleaseMutex(client->m_syncPrimitives->mutex);
				break;
			case TypeSyncPrimitives::NOT:
				client->m_bank->UpdateClientBalance(*client, GetBalanceChangeValue());
				break;
		}
		
	}
	
	return 0;
}

unsigned int CBankClient::GetSleepDuration(CBankClient *client)
{
	return (1000 + rand() % 3000) * (client->m_id + 1);
}


unsigned int CBankClient::GetBalanceChangeValue()
{
	return (-100 + rand() % 201);
}