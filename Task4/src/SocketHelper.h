#pragma once


class CSocketHelper
{
public:
	CSocketHelper() = default;
	bool CreateSocket(std::string const & address);
	bool BindSocket();
	bool ConnectSocket();
	bool SendMes(std::string const &message);
	bool WaitSend(std::vector<std::string> &messages, size_t messageNumber);


	~CSocketHelper();
private:
	struct addrinfo* m_addr = NULL;
	SOCKET m_socket;
};