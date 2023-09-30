#include "PostgresDatabase.h"

int PostgresDatabase::connect(s_dbConfig config)
{
	try {
		if (m_connection == nullptr) {
			std::ostringstream s;
			s	<< "user=" << config.user << " "
				<< "password=" << config.password << " "
				<< "dbname=" << config.dbname << " "
				<< "host=" << config.host << " "
				<< "port=" << config.port << " ";
			//std::string credentials = std::format("user='{}' password='{}' dbname='{}' hostaddr='{}' port='{}'",
												//config.user, config.password, config.dbname, config.host, config.port);
			std::string credentials = s.str();
			m_connection = std::make_shared<pqxx::connection>(credentials);
		}
	}
	catch (std::exception& ex) {
		m_connection = nullptr;
		return 1;
	}
	
	return 0;
}

int PostgresDatabase::disconnect()
{
	m_connection->close();
	return 0;
}

pqxx::result PostgresDatabase::executeQuery(std::string query)
{
	pqxx::work tx{ *m_connection };
	try {
		pqxx::result res = tx.exec(query);
		return res;
	}
	catch (std::exception& ex) {
		return pqxx::result();
	}
}

pqxx::result PostgresDatabase::executeCommand(std::string command)
{
	pqxx::work tx{ *m_connection };
	try {
		pqxx::result res = tx.exec(command);
		tx.commit();
		return res;
	}
	catch (std::exception& ex) {
		return pqxx::result();
	}
}
