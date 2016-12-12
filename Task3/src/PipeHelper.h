#pragma once

struct SPipeHelper
{
	static void SendMes(std::string const &message);
	static void WaitSend(std::vector<std::string> &messages, size_t messageNumber);
};