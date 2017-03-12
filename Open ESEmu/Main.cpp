#include <iostream>
#include <stdint.h>
#include "Common\Configuration\ConfigReader.h"
#include "Network\Server.h"
#include "Common\Database\DBHandler.h"
#include <boost\thread.hpp>
#include <conio.h>
#include "Common\LUA Engine\LuaAPI.h"
#include "Common\LUA Engine\LuaInterface.h"
#include "Common\LUA Engine\Callback.h"
#include "Common\LUA Engine\CVarSystem\CmdLineInterface.h"
#include "Common\Logger\Logger.h"

void main(int argc, char* argv[])
{
	SetConsoleTitle("Open ESEmu");
	printf("[ Open ESEmu ]\nMade by d3vil401 (http://d3vsite.org), author of ESEmu Project\nBuild %s %s\n\n", __DATE__, __TIME__);


	g_pCmdLineInterface = new CmdLineInterface();
	g_pCmdLineInterface->RegisterCommand("-PerformCryptoBenchmark");
	g_pCmdLineInterface->GetCommandsFromCmdLine(argc, argv);

	

	Log("Loading settings...");
	if (!g_ConfigManager->getInstance()->load())
	{
		LogError("Settings not loaded, aborting.");
		_getch();
		return;
	}

	Log("Connecting to the database %s@%s:%d/%s...", 
		  g_ConfigManager->getInstance()->getString(g_ConfigManager->DB_USER).c_str()
		, g_ConfigManager->getInstance()->getString(g_ConfigManager->DB_HOST).c_str()
		, g_ConfigManager->getInstance()->getNumber(g_ConfigManager->DB_PORT)
		, g_ConfigManager->getInstance()->getString(g_ConfigManager->DB_DATABASE).c_str()
	);

	if (!g_Database->getInstance()->isConnected())
	{
		LogError("Can't connect to the server!");
		printf("%s\n", g_Database->getInstance()->lastError().c_str());
		_getch();
		return;
	} else Log("Successfully connected to the database.");
	g_Database->getInstance()->printVersion();

	Log("Setting up cryptography basics...");
	

	Log("Initializing Script Engine...");
	g_luaEngine = new ScriptEngine::LuaCallback();

	if (g_ConfigManager->getInstance()->getBoolean(g_ConfigManager->RESTAPI_ENABLE))
	{
		Log("Enabled REST APIs, starting up HTTP Server...");
		//_httpServerPort = (unsigned int*)malloc(sizeof(unsigned int));
		//memset(_httpServerPort, 0x00, sizeof(unsigned int));
		//*_httpServerPort = g_ConfigManager->getInstance()->getNumber(g_ConfigManager->HTTP_PORT);
		//_httpServer = new HTTPServer();
		//boost::thread restapi(&restStartup);
	}

	Log("Starting server...");

	_server->Initialize(g_ConfigManager->getInstance()->getNumber(g_ConfigManager->MAX_CLIENTS));

	g_Database->getInstance()->~DBHandler();
	Log("Server stopped.\n");
	_getch();
}