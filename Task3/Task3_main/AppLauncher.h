#pragma once

enum class DataTransferType
{
	Pipe,
	Socket
};

class CAppLauncher
{
public:
	CAppLauncher(char *argv[], DataTransferType type);
	void Run();
	~CAppLauncher();
private:
	void WaitMessages();
	void SettingProcess(STARTUPINFO &si);
	
private:
	DataTransferType m_type;
	size_t m_procNumber;
	size_t m_itNumber;
	std::vector<STARTUPINFO> m_startUpInfos;
	std::vector<PROCESS_INFORMATION> m_processesInformation;
};