#include "stdafx.h"
#include "Config.h"
#include <string>

BOOL CConfig::LoadConfig(const CString& config_file_path)
{
	CFile			file;
	ULONGLONG		FileSize;
	CArray<CHAR>    buff;
	

	if (!file.Open(config_file_path, CFile::modeRead))
		return FALSE;

	FileSize = file.GetLength();
	buff.SetSize(FileSize);
	file.Read(buff.GetData(), FileSize);
	file.Close();

	if (buff.GetSize() == 0)
	{
		return FALSE;
	}

	if (!ParseConfig(buff.GetData(), m_config))
	{
		return FALSE;
	}

	return TRUE;
}

VOID CConfig::SaveConfig(const CString & config_file_path) const
{
	CFile     file;
	std::string config = WriteConfig(m_config);
	
	if (!file.Open(config_file_path, CFile::modeWrite|CFile::modeCreate))
		return;

	file.Write(config.c_str(), config.length());
	file.Close();
}
