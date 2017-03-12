#include "DBHandler.h"

DBHandler* DBHandler::__instance = nullptr;

DBHandler* DBHandler::getInstance()
{
	if (__instance != nullptr)
		return __instance;
	else
		return __instance = new DBHandler();
}

DBHandler* g_Database = nullptr;