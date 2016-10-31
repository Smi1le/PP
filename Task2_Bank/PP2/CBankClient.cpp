#include "CBankClient.h"

CBankClient::CBankClient(CBank *bank, unsigned int id, HANDLE &mutex, bool nf)
{
	m_isUsingNormalForm = nf;
	m_bank = bank;
	m_id = id;
	m_hMutex = mutex;
	m_handle = CreateThread(NULL, 0, ThreadFunction, this, 0, NULL);
}


unsigned int CBankClient::GetId()
{
	return m_id;
}

void SomeLongOperation()
{
	size_t number = rand() % 1000001;
	int count = 0;
	for (size_t i = 0; i != number; ++i) { count++; }
}

DWORD WINAPI CBankClient::ThreadFunction(LPVOID lpParam)
{
	
	CBankClient *client = (CBankClient*)lpParam;
	
	srand(client->m_id);
	
	while (true)
	{
		if (client->m_isUsingNormalForm)
		{
			WaitForSingleObject(client->m_hMutex, INFINITE);
			client->m_bank->UpdateClientBalance(*client, GetBalanceChangeValue());
			ReleaseMutex(client->m_hMutex);
		}
		else
		{
			client->m_bank->UpdateClientBalance(*client, GetBalanceChangeValue());
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