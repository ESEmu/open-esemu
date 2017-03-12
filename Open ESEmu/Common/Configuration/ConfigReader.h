#pragma once
#ifndef _CONFIGHANDLER_H_
#define _CONFIGHANDLER_H_

extern "C" 
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}
#include <string>
#include <stdint.h>
#include <iostream>
#pragma comment (lib, "lua53.lib")

class ConfigManager
{
private:
	static ConfigManager* __instance;

public:
	static ConfigManager* getInstance();

	ConfigManager();
	~ConfigManager();

	enum boolean_config_t 
	{
		DB_SSL_ENABLE,
		TEST_SERVER,
		RESTAPI_ENABLE,
		HTTP_WHITELIST_ENABLE,
		HTTP_ENABLE_SSL,
		SYNC_ENABLE,
		SEED_ENCRYPTED,
		LAST_BOOLEAN_CONFIG
	};

	enum string_config_t 
	{
		DB_PUBLIC_KEY,
		DB_PUBLIC_CERT,
		DB_CA_KEY,
		DB_HOST,
		DB_USER,
		DB_PASSWORD,
		DB_DATABASE,
		CLIENT_VERSION,
		SERVER_NAME,
		SERVER_IP,
		HTTP_WHITELIST,
		HTTP_SSL_PRIVKEY,
		HTTP_SSL_PRIVCRT,
		SYNC_IP,
		SYNC_KEY,
		DEF_DES,
		DEF_HMAC,
		SEED_FILE,
		SEED_KEY,
		LAST_STRING_CONFIG
	};

	enum integer_config_t 
	{
		SERVER_PORT,
		DB_PORT,
		MAX_CLIENTS,
		HTTP_MAX_CLIENTS,
		HTTP_PORT,
		HTTP_AUTHTYPE,
		VPROTOCOL_VERSION,
		SYNC_ROLE,
		SYNC_TYPE,
		SYNC_PORT,
		SEED_SIZE,
		MAX_TCP_WINDOW,
		LAST_INTEGER_CONFIG
	};

	bool load();

	const std::string& getString(string_config_t _what) const;
	int32_t getNumber(integer_config_t _what) const;
	bool getBoolean(boolean_config_t _what) const;

private:
	static std::string getGlobalString(lua_State* _L, const std::string& _identifier, const std::string& _default = "");
	static int32_t getGlobalNumber(lua_State* _L, const std::string& _identifier, const int32_t _default = 0);
	static std::string getGlobalStringField(lua_State* _L, const std::string& _identifier, const int32_t _key, const std::string& _default = "");

	bool m_isLoaded;
	std::string StringCFG[LAST_STRING_CONFIG];
	int32_t IntegerCFG[LAST_INTEGER_CONFIG];
	bool BoolCFG[LAST_BOOLEAN_CONFIG];
};

extern ConfigManager* g_ConfigManager;

#endif