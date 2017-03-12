#include "Libraries\LibServer.as"
#include "Libraries\LibDatabase.as"
#include "_restConstants.as"

void main()
{
	_sessionIndex++;
	print("Session " + _sessionIndex + "\n");
	server[_sessionIndex].setGlobalCache("ACTIVE", "1");
}

void run()
{
	print("[HTTP] Run\n");
}

void end()
{
	
}