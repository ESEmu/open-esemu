// DO NOT REMOVE, NEEDED TO LOAD CONSTANTS
#include "Constants.as"
#include "Libraries\LibDatabase.as"
#include "Libraries\LibServer.as"

// THE MAIN FILE CAN BE EDITED TO ADD MORE STUFF,
// BUT FOR THE SAKE OF GOD DON'T REMOVE STUFF OR EDIT IT
// IF YOU DON'T KNOW WHAT YOU'RE DOING.

void main()
{
	if ( EXECUTE_TESTS )
	{
		string res = db.execQuery("SELECT \"1\";");
		if (res.length > 0)
			print("Scripted database interface working correctly.\n");
		
			
		serv.setGlobalCache("TESTSCRIPT", "valid");
		res = serv.getGlobalCache("TESTSCRIPT");
		if (res.length > 0)
			print("Scripted cache interface working correctly.\n");
		
	} else
		print("EXECUTE_TESTS is disabled, skipping tests.\n");
		
}

void run()
{
	
}

void end()
{
	//print("[MAIN SCRIPT] end\n");
}