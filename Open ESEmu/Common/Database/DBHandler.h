#pragma once
#ifndef _DBHANDLER_H_
#define _DBHANDLER_H_

#include <iostream>
#include "..\Data\Database\Queries.h"
#include "..\Configuration\ConfigReader.h"
#include "..\Data\Database\Structures.h"
#include "..\Logger\Logger.h"
#include <string>
#include <mysql.h>
#pragma comment (lib, "libmysql.lib")
using namespace std;
#pragma warning (disable: 4251)
#pragma warning (disable: 4996)
#pragma warning (disable: 4477)
#pragma warning (disable: 4305)
#pragma warning (disable: 4715)
#pragma warning (disable: 4473)
#pragma warning (disable: 4474)

#define FREE mysql_free_result(Result);


class DBHandler
{
private:

	string			_dbHost;
	string			_dbUser;
	string			_dbPassword;
	string			_dbDatabase;
	unsigned int	_dbPort;
	bool			_dbSSLEnable;
	string			_dbPublicKey;
	string			_dbPublicCert;
	string			_dbCAKey;

	bool			_isConnected = false;

	MYSQL*			_serverInstance = nullptr;
	MYSQL*			_dbInstance = nullptr;

	static DBHandler* __instance;

	int _query(const char* query)
	{
		return mysql_query(this->_dbInstance, query);
	}

public:

	static DBHandler* getInstance();

	void printVersion()
	{
		MYSQL_RES* Result;
		MYSQL_ROW  Row;
		char Query[256] = { 0 };

		sprintf(Query, "SELECT VERSION();");
		if (mysql_query(this->_dbInstance, Query))
		{
			LogError("MySQL Error: %s.", mysql_error(this->_dbInstance));
			return;
		}
		Result = mysql_store_result(this->_dbInstance);

		if ((Row = mysql_fetch_row(Result)))
		{
			Log("MySQL Server %s.", Row[0]);
		}

		Log("Using MySQL Client %s.", mysql_get_client_info());
	}

	DBHandler()
	{
		this->_dbHost = g_ConfigManager->getInstance()->getString(g_ConfigManager->DB_HOST);
		this->_dbUser = g_ConfigManager->getInstance()->getString(g_ConfigManager->DB_USER);
		this->_dbPassword = g_ConfigManager->getInstance()->getString(g_ConfigManager->DB_PASSWORD);
		this->_dbDatabase = g_ConfigManager->getInstance()->getString(g_ConfigManager->DB_DATABASE);
		this->_dbPort = g_ConfigManager->getInstance()->getNumber(g_ConfigManager->DB_PORT);
		this->_dbSSLEnable = g_ConfigManager->getInstance()->getBoolean(g_ConfigManager->DB_SSL_ENABLE);
		this->_dbPublicKey = g_ConfigManager->getInstance()->getString(g_ConfigManager->DB_PUBLIC_KEY);
		this->_dbPublicCert = g_ConfigManager->getInstance()->getString(g_ConfigManager->DB_PUBLIC_CERT);
		this->_dbCAKey = g_ConfigManager->getInstance()->getString(g_ConfigManager->DB_CA_KEY);

		this->_serverInstance = mysql_init(this->_serverInstance);

		if (this->_dbSSLEnable)
		{
			char __path[MAX_PATH] = { 0 };
			sprintf_s(__path, "data\\MySQL\\%s", this->_dbPublicKey.c_str());

			this->_dbPublicKey.assign(__path);
			sprintf_s(__path, "data\\MySQL\\%s", this->_dbPublicCert.c_str());
			this->_dbPublicCert.assign(__path);
			sprintf_s(__path, "data\\MySQL\\%s", this->_dbCAKey.c_str());
			this->_dbCAKey.assign(__path);

			mysql_ssl_set(this->_serverInstance, this->_dbPublicKey.c_str(),
												 this->_dbPublicCert.c_str(),
												 this->_dbCAKey.c_str(),
												 NULL, NULL);
		}
		
		this->_dbInstance = mysql_real_connect(this->_serverInstance, this->_dbHost.c_str(), this->_dbUser.c_str(),
												this->_dbPassword.c_str(), this->_dbDatabase.c_str(), this->_dbPort,
												NULL, 0);
		if (this->_dbInstance != NULL) 
		{
			this->_isConnected = true;
			mysql_autocommit(this->_dbInstance, true);
		} else
			this->_isConnected = false;
	}

	~DBHandler()
	{
		Log("Stopping connection to MySQL Server...");
		mysql_close(this->_serverInstance);
		mysql_close(this->_dbInstance);
	}

	bool isConnected()
	{
		return this->_isConnected;
	}

	string lastError()
	{
		string error(mysql_error(this->_dbInstance));
		return error;
	}

	void authAccount(string Username, string Password, USERAUTH_INFO& out)
	{
		MYSQL_RES* Result;
		MYSQL_ROW Row;
		char Query[256] = { 0 };

		sprintf(Query, QUERY_SELECT_ACCOUNT_LOGIN, Username.c_str(), Password.c_str());
		if (_query(Query))
		{
			LogError("MySQL Error: %s.", mysql_error(this->_dbInstance));
			return;
		}

		out = *(USERAUTH_INFO*)malloc(sizeof(USERAUTH_INFO));

		Result = mysql_store_result(this->_dbInstance);
		Row = mysql_fetch_row(Result);

		/*
		
		MYSQL_TIME	 _joinDate;
		char*		 _status;
		char*		 _authLevel;
		MYSQL_TIME	 _lastLogin;
		char*		 _securityPIN;
		MYSQL_TIME	 _bannedUntil;
		
		*/
		if (Row != NULL)
		{
			out._id = atoi(Row[0]);
			out._email = (char*)Row[1];
			out._password = (char*)Row[2];
			out._status = (char*)Row[4];
			out._authLevel = (char*)Row[5];
			out._securityPIN = (char*)Row[7];
			// TODO: cast MYSQL_TIME
		}

		mysql_free_result(Result);
	}

	bool checkNameAvail(string _nickname)
	{
		MYSQL_RES* Result;
		MYSQL_ROW Row;
		char Query[256] = { 0 };

		sprintf(Query, "SELECT ID FROM Characters WHERE Nickname = '%s';", _nickname.c_str());
		if (mysql_query(this->_dbInstance, Query))
		{
			LogError("MySQL Error: %s.", mysql_error(this->_dbInstance));
			return -1;
		}
		Result = mysql_store_result(this->_dbInstance);

		if ((Row = mysql_fetch_row(Result)))
		{
			if (atoi(Row[0]) > 0)
				return false;
			else
				return true;
		}
		return false;
	}

	void executeQuery(string _query, string& out)
	{
		MYSQL_RES* Result;
		MYSQL_ROW Row;
		char Query[256] = { 0 };

		sprintf(Query, _query.c_str());
		if (mysql_query(this->_dbInstance, Query))
		{
			LogError("MySQL Error: %s.", mysql_error(this->_dbInstance));
			out = "";
		}
		Result = mysql_store_result(this->_dbInstance);

		if ((Row = mysql_fetch_row(Result)))
		{
			if (Row[0])
			{
				string res(Row[0]);
				out = res;
				return;
			} else {
				out = "";
				return;
			}
		}
	}

	string getCurrentPlayerNickname(unsigned int _id)
	{
		MYSQL_RES* Result;
		MYSQL_ROW Row;
		char Query[256] = { 0 };

		sprintf(Query, "SELECT Nickname FROM Characters WHERE OwnerID = '%d';", _id);
		if (mysql_query(this->_dbInstance, Query))
		{
			LogError("MySQL Error: %s.", mysql_error(this->_dbInstance));
			return "";
		}
		Result = mysql_store_result(this->_dbInstance);

		if ((Row = mysql_fetch_row(Result)))
		{
			if (Row[0])
			{
				string res(Row[0]);
				return res;
			} else
				return "";
		}
	}

	string getBannedDate_YM(unsigned int _id)
	{
		MYSQL_RES* Result;
		MYSQL_ROW Row;
		char Query[256] = { 0 };

		sprintf(Query, QUERY_SELECT_BAN_EXPIRE, _id);
		if (_query(Query))
		{
			LogError("MySQL Error: %s.", mysql_error(this->_dbInstance));
			return "";
		}
		Result = mysql_store_result(this->_dbInstance);

		if ((Row = mysql_fetch_row(Result)) != NULL)
		{
			string res(Row[0]);
			return res;
		} else
			return "";
	}

	void checkHTTPAuth(string _username, string _password, string& _out)
	{
		MYSQL_RES* Result;
		MYSQL_ROW Row;
		char Query[256] = { 0 };

		sprintf(Query, QUERY_SELECT_BASIC_AUTH, _username.c_str(), _password.c_str());
		if (_query(Query))
		{
			LogError("MySQL Error: %s.", mysql_error(this->_dbInstance));
			_out = "-1";
			return;
		}
		Result = mysql_store_result(this->_dbInstance);

		if ((Row = mysql_fetch_row(Result)) != NULL)
			_out = Row[0];
		else
			_out = "-1";
	}

	void checkHTTPAuthToken(string _username, string _token, string& _out)
	{
		MYSQL_RES* Result;
		MYSQL_ROW Row;
		char Query[256] = { 0 };

		sprintf(Query, QUERY_SELECT_TOKEN_AUTH, _username.c_str(), _token.c_str());
		if (_query(Query))
		{
			LogError("MySQL Error: %s.", mysql_error(this->_dbInstance));
			_out = "-1";
			return;
		}
		Result = mysql_store_result(this->_dbInstance);

		if ((Row = mysql_fetch_row(Result)) != NULL)
			_out = Row[0];
		else
			_out = "-1";
	}

};

extern DBHandler* g_Database;

#endif