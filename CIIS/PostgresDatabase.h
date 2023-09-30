#pragma once
#include "IDatabase.h"
#include <pqxx/pqxx>
class PostgresDatabase :
    public IDatabase
{
    std::shared_ptr<pqxx::connection> m_connection;

public:
    virtual int connect(s_dbConfig config);
    virtual int disconnect();

    pqxx::result executeQuery(std::string query);
    pqxx::result executeCommand(std::string command);
};

