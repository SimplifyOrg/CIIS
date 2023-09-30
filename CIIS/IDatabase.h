#pragma once
#include "DatabaseConfiguration.h"
class IDatabase
{
public:
	virtual int connect(s_dbConfig config) = 0;
	virtual int disconnect() = 0;
};

