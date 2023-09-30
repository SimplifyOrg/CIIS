#pragma once
#include <string>

struct DatabaseConfiguration {
    std::string user;
    std::string host;
    std::string password;
    std::string dbname;
    std::string port;
};

typedef struct DatabaseConfiguration s_dbConfig;
