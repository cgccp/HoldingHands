#pragma once

#include "configuration.h"

/*
config format....
#
key = value

.....这东西直接拿json就可以了.....
*/

class CConfig
{
private:
	Config m_config;

public:
	BOOL LoadConfig(const CString & config_file_path);
	VOID SaveConfig(const CString & config_file_path) const;

	Config& cfg()
	{ 
		return m_config;
	};

	CConfig(const CString & config_file_path)
	{
		if (!LoadConfig(config_file_path))
		{
			AfxMessageBox(TEXT("Load Config failed!"), MB_OK | MB_ICONERROR);
			exit(1);
		}
	}

	CConfig(){}

	~CConfig(){}
};
