#pragma once


class CSocketHelper
{
public:
	CSocketHelper() = default;
	bool CreateSocket();
	bool TuneSocket(int port, ULONG address);
	bool TuneSocket(int port, std::string const &address);
	bool SendMes(std::string const &message);
	bool WaitSend(std::vector<std::string> &messages, size_t messageNumber);


	~CSocketHelper();
private:

	SOCKET m_socket;
};