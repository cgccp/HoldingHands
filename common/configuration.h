#pragma once

#include <vector>
#include <map>
#include <string>
#include <cassert>
#include <sstream>
#include <iomanip>
#include <stdexcept>

#include "json\json.h"

#ifdef _DEBUG
#pragma comment(lib,"jsond.lib")
#else
#pragma comment(lib,"json.lib")
#endif

typedef Json::Value Config;

#define WriteConfig(cfg) Json::StyledWriter().write(cfg)

#define ParseConfig(doc, root) Json::Reader().parse(doc, root, true)
