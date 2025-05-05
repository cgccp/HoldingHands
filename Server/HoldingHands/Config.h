#pragma once

#include "configuration.h"

/*
config format....
#
key = value

.....这东西直接拿json就可以了.....
TODO: JSON 这玩意写配置未来会去掉.
*/

class CConfig
{
private:
	Config m_config;

public:
	CString GetStr(const std::string& group, const std::string& key, const std::string &def="") {
		auto &node = group.empty() ? m_config : m_config[group];
		return node.isObject() ? CString(node[key].isNull() ? def.c_str() : node[key].asCString()) : CString(def.c_str());
	}
	void SetStr(const std::string& group, const std::string& key, const std::string &value) {
		auto &node = group.empty() ? m_config : m_config[group];
		node[key] = value;
	}

	int GetInt(const std::string& group, const std::string& key, int def = 0) {
		auto &node = group.empty() ? m_config : m_config[group];
		return node.isObject() ? node[key].isNull() ? def : node[key].isString() ? atoi(node[key].asCString()) : node[key].asInt() : def;
	}
	void SetInt(const std::string& group, const std::string& key, int value) {
		auto& node = group.empty() ? m_config : m_config[group];
		node[key] = value;
	}

	void SetMember(const std::string& member, const Config& cfg) {
		m_config[member] = cfg;
	}
	Config GetMember(const std::string& member) const {
		return m_config[member];
	}

	BOOL LoadConfig(const CString & config_file_path);
	VOID SaveConfig(const CString & config_file_path) const;

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
