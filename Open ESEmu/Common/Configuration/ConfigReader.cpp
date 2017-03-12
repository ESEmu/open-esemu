#include "ConfigReader.h"
#include <stdexcept>
#include <iostream>
#pragma warning (disable: 4172)
#pragma warning (disable: 4005)
#pragma warning (disable: 4081)
#pragma warning (disable: 4477)
#pragma warning (disable: 4018)

#if LUA_VERSION_NUM >= 502
	#define lua_strlen lua_rawlen
#endif

ConfigManager* ConfigManager::__instance = nullptr;

ConfigManager* ConfigManager::getInstance()
{
	if (__instance != nullptr)
		return __instance;
	else return __instance = new ConfigManager();
}

bool booleanString(const std::string& str)
{
	if (str.empty()) {
		return false;
	}

	char ch = tolower(str.front());
	return ch != 'f' && ch != 'n' && ch != '0';
}

ConfigManager::ConfigManager()
{
	this->m_isLoaded = false;
}

ConfigManager::~ConfigManager()
{
	//
}

bool ConfigManager::load()
{
	lua_State* L = luaL_newstate();
	if (!L)
	{
		printf("Can't allocate memory.\n");
		throw std::runtime_error("Failed to allocate memory");
	}

	luaL_openlibs(L);
	if (luaL_dofile(L, "Settings.lua"))
	{
		printf("[!] Cannot open file.\n%s\n", lua_tostring(L, -1));
		lua_close(L);
		return false;
	}

	// parse config
		if (!this->m_isLoaded)
		{

			// ------- LICENSE & INTERNAL
			this->StringCFG[SERVER_IP] = this->getGlobalString(L, "ServerIP", "127.0.0.1");
			this->StringCFG[SERVER_NAME] = this->getGlobalString(L, "ServerName", "d3vil401");
			this->BoolCFG[TEST_SERVER] = booleanString(this->getGlobalString(L, "TestServer", "no"));
			this->IntegerCFG[MAX_CLIENTS] = this->getGlobalNumber(L, "MaxClients", 50);
			this->IntegerCFG[SERVER_PORT] = this->getGlobalNumber(L, "ServerPort", 1);
			this->IntegerCFG[VPROTOCOL_VERSION] = this->getGlobalNumber(L, "vProtocol", 2);
			this->IntegerCFG[MAX_TCP_WINDOW] = this->getGlobalNumber(L, "MaxTCPWindow", 2048);

			this->BoolCFG[RESTAPI_ENABLE] = booleanString(this->getGlobalString(L, "RESTAPIEnable", "no"));
			this->IntegerCFG[HTTP_AUTHTYPE] = this->getGlobalNumber(L, "RESTAuthType", 1);
			this->IntegerCFG[HTTP_PORT] = this->getGlobalNumber(L, "RESTAPIPort", 3401);
			this->BoolCFG[HTTP_WHITELIST_ENABLE] = booleanString(this->getGlobalString(L, "RESTWhiteListEnable", "yes"));
			this->IntegerCFG[HTTP_MAX_CLIENTS] = this->getGlobalNumber(L, "RESTMaxSessions", 1);
			this->StringCFG[HTTP_WHITELIST] = this->getGlobalString(L, "RESTWhiteList", "httpwl.lst");
			this->BoolCFG[HTTP_ENABLE_SSL] = booleanString(this->getGlobalString(L, "RESTEnableSSL", "no"));
			this->StringCFG[HTTP_SSL_PRIVCRT] = this->getGlobalString(L, "RESTPrivateCert", "http-cert.pem");
			this->StringCFG[HTTP_SSL_PRIVKEY] = this->getGlobalString(L, "RESTPrivateKey", "http-privkey.pem");

			this->BoolCFG[SYNC_ENABLE] = booleanString(this->getGlobalString(L, "CSyncEnable", "no"));
			this->IntegerCFG[SYNC_ROLE] = this->getGlobalNumber(L, "CSyncRole", 3);
			this->IntegerCFG[SYNC_TYPE] = this->getGlobalNumber(L, "CSyncType", 2);
			this->IntegerCFG[SYNC_PORT] = this->getGlobalNumber(L, "CSyncPort", 50);
			this->StringCFG[SYNC_IP] = this->getGlobalString(L, "CSyncIP", "127.0.0.1");
			this->StringCFG[SYNC_KEY] = this->getGlobalString(L, "CSyncKey", "d3vil401d3vil401");

			this->BoolCFG[SEED_ENCRYPTED] = booleanString(this->getGlobalString(L, "cPRNGEncrypted", "no"));
			this->StringCFG[DEF_DES] = this->getGlobalString(L, "cDefaultDES", "C7D8C4BFB5E9C0FD");
			this->StringCFG[DEF_HMAC] = this->getGlobalString(L, "cDefaultHMAC", "C0D3BDC3B7CEB8B8");
			this->StringCFG[SEED_FILE] = this->getGlobalString(L, "cPRNGFile", "_seed.d3v");
			this->StringCFG[SEED_KEY] = this->getGlobalString(L, "cPRNGKey", "testtest");
			this->IntegerCFG[SEED_SIZE] = this->getGlobalNumber(L, "cPRNGSize", 2048);


			// ----------------

			// ------- DATABASE
			this->BoolCFG[TEST_SERVER] = booleanString(this->getGlobalString(L, "DBSSL", "no"));
			this->StringCFG[DB_PUBLIC_KEY] = this->getGlobalString(L, "DBPubKey", "mysqlkey.pub");
			this->StringCFG[DB_HOST] = this->getGlobalString(L, "DBHost", "127.0.0.1");
			this->IntegerCFG[DB_PORT] = this->getGlobalNumber(L, "DBPort", 3304);
			this->StringCFG[DB_USER] = this->getGlobalString(L, "DBUser", "root");
			this->StringCFG[DB_PASSWORD] = this->getGlobalString(L, "DBPass", "");
			this->StringCFG[DB_DATABASE] = this->getGlobalString(L, "DBDatabase", "ESEmu");
			// ----------------

			// ------- CLIENT
			this->StringCFG[CLIENT_VERSION] = getGlobalString(L, "ClientVersion", "V4.1112.6");
			// --------------
		}

		// ------- LICENSE & INTERNAL
		this->StringCFG[SERVER_IP] = this->getGlobalString(L, "ServerIP", "127.0.0.1");
		this->StringCFG[SERVER_NAME] = this->getGlobalString(L, "ServerName", "d3vil401");
		this->BoolCFG[TEST_SERVER] = booleanString(this->getGlobalString(L, "TestServer", "no"));
		this->IntegerCFG[MAX_CLIENTS] = this->getGlobalNumber(L, "MaxClients", 50);
		this->IntegerCFG[SERVER_PORT] = this->getGlobalNumber(L, "ServerPort", 1);
		this->IntegerCFG[MAX_TCP_WINDOW] = this->getGlobalNumber(L, "MaxTCPWindow", 2048);

		this->BoolCFG[RESTAPI_ENABLE] = booleanString(this->getGlobalString(L, "RESTAPIEnable", "no"));
		this->IntegerCFG[HTTP_AUTHTYPE] = this->getGlobalNumber(L, "RESTAuthType", 1);
		this->IntegerCFG[HTTP_PORT] = this->getGlobalNumber(L, "RESTAPIPort", 3401);
		this->BoolCFG[HTTP_WHITELIST_ENABLE] = booleanString(this->getGlobalString(L, "RESTWhiteListEnable", "yes"));
		this->IntegerCFG[HTTP_MAX_CLIENTS] = this->getGlobalNumber(L, "RESTMaxSessions", 1);
		this->StringCFG[HTTP_WHITELIST] = this->getGlobalString(L, "RESTWhiteList", "httpwl.lst");
		this->BoolCFG[HTTP_ENABLE_SSL] = booleanString(this->getGlobalString(L, "RESTEnableSSL", "no"));
		this->StringCFG[HTTP_SSL_PRIVCRT] = this->getGlobalString(L, "RESTPrivateCert", "http-cert.pem");
		this->StringCFG[HTTP_SSL_PRIVKEY] = this->getGlobalString(L, "RESTPrivateKey", "http-privkey.pem");

		this->BoolCFG[SYNC_ENABLE] = booleanString(this->getGlobalString(L, "CSyncEnable", "no"));
		this->IntegerCFG[SYNC_ROLE] = this->getGlobalNumber(L, "CSyncRole", 3);
		this->IntegerCFG[SYNC_TYPE] = this->getGlobalNumber(L, "CSyncType", 2);
		this->IntegerCFG[SYNC_PORT] = this->getGlobalNumber(L, "CSyncPort", 50);
		this->StringCFG[SYNC_IP] = this->getGlobalString(L, "CSyncIP", "127.0.0.1");
		this->StringCFG[SYNC_KEY] = this->getGlobalString(L, "CSyncKey", "d3vil401d3vil401");

		this->BoolCFG[SEED_ENCRYPTED] = booleanString(this->getGlobalString(L, "cPRNGEncrypted", "no"));
		this->StringCFG[DEF_DES] = this->getGlobalString(L, "cDefaultDES", "C7D8C4BFB5E9C0FD");
		this->StringCFG[DEF_HMAC] = this->getGlobalString(L, "cDefaultHMAC", "C0D3BDC3B7CEB8B8");
		this->StringCFG[SEED_FILE] = this->getGlobalString(L, "cPRNGFile", "_seed.d3v");
		this->StringCFG[SEED_KEY] = this->getGlobalString(L, "cPRNGKey", "testtest");
		this->IntegerCFG[SEED_SIZE] = this->getGlobalNumber(L, "cPRNGSize", 2048);
		// ----------------

		// ------- DATABASE
		this->StringCFG[DB_HOST] = this->getGlobalString(L, "DBHost", "127.0.0.1");
		this->IntegerCFG[DB_PORT] = this->getGlobalNumber(L, "DBPort", 3304);
		this->StringCFG[DB_USER] = this->getGlobalString(L, "DBUser", "root");
		this->StringCFG[DB_PASSWORD] = this->getGlobalString(L, "DBPass", "");
		this->StringCFG[DB_DATABASE] = this->getGlobalString(L, "DBDatabase", "ESEmu");
		// ----------------

		// ------- CLIENT
		this->StringCFG[CLIENT_VERSION] = getGlobalString(L, "ClientVersion", "V4.1112.6");
		// --------------
		
		this->m_isLoaded = true;
		//lua_close(L);

		return true;
}

const std::string& ConfigManager::getString(string_config_t _what) const
{
	char* Noll = "NULL";
	if (this->m_isLoaded && _what < LAST_STRING_CONFIG) {
		return StringCFG[_what];
	}
	else {
		printf("Warning about ConfigManager::getString() -> %s", StringCFG[_what].c_str());
		return Noll;
	}
}

int32_t ConfigManager::getNumber(integer_config_t _what) const
{
	if (this->m_isLoaded && _what < LAST_INTEGER_CONFIG) {
		return IntegerCFG[_what];
	}
	else {
		//CM->Write(LOG_WARN, "Warning about ConfigManager::getNumber() -> %s", _what);
		return 0;
	}
}

bool ConfigManager::getBoolean(boolean_config_t _what) const
{
	if (this->m_isLoaded && _what < LAST_BOOLEAN_CONFIG) {
		return BoolCFG[_what];
	}
	else {
		//CM->Write(LOG_WARN, "Warning about ConfigManager::getBoolean() -> %s", _what);
		return false;
	}
}

std::string ConfigManager::getGlobalString(lua_State* _L, const std::string& _identifier, const std::string& _default)
{
	lua_getglobal(_L, _identifier.c_str());

	if (!lua_isstring(_L, -1)) {
		return _default;
	}

	size_t len = lua_strlen(_L, -1);
	std::string ret(lua_tostring(_L, -1), len);
	lua_pop(_L, 1);
	return ret;
}

int32_t ConfigManager::getGlobalNumber(lua_State* _L, const std::string& _identifier, const int32_t _default)
{
	lua_getglobal(_L, _identifier.c_str());

	if (!lua_isnumber(_L, -1)) {
		return _default;
	}

	int32_t val = (int32_t)lua_tonumber(_L, -1);
	lua_pop(_L, 1);
	return val;
}

std::string ConfigManager::getGlobalStringField(lua_State* _L, const std::string& _identifier, const int32_t _key, const std::string& _default)
{
	lua_getglobal(_L, _identifier.c_str());

	lua_pushnumber(_L, _key);
	lua_gettable(_L, -2);  /* get table[key] */

	if (!lua_isstring(_L, -1)) {
		return _default;
	}

	std::string result = lua_tostring(_L, -1);
	lua_pop(_L, 2);  /* remove number and key*/
	return result;
}

ConfigManager* g_ConfigManager = new ConfigManager();