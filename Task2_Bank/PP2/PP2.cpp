#include "stdafx.h"
#include <iostream>
#include "CBank.h"
#include "CBankClient.h"

int main()
{
	CBank* bank = new CBank();
	CBankClient* client1 = bank->CreateClient();
	CBankClient* client2 = bank->CreateClient();

	while (true)
	{
		WaitForMultipleObjects(static_cast<DWORD>(bank->GetNumberClients()), bank->GetHandles(), TRUE, INFINITE);
	}

    return 0;
}