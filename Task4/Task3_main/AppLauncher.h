#pragma once


class CAppLauncher
{
public:
	CAppLauncher(char *argv[]);
	void Run();
	~CAppLauncher();
private:
	void WaitMessages();
	void SettingProcess(STARTUPINFO &si);
	
private:
	size_t m_procNumber;
	size_t m_itNumber;
	std::vector<STARTUPINFO> m_startUpInfos;
	std::vector<PROCESS_INFORMATION> m_processesInformation;
	std::string m_ipAddress;
};