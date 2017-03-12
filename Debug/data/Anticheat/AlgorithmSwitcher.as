#include "DeterministicAI.as"
#include "..\Constants.as"

void acEventHandler (array<uint> &in infoList)
{
	switch (ACM_USE_ALGORITHM)
	{
		case ACM_DETERMINISTICAI:
			print("Using deterministic AI.\n");
			break;
		
		case ACM_PARAMCHECKER:
			print("Using param checker.\n");
			break;
			
		case ACM_MAPGUARDAI:
			print("Using Map Guard AI.\n");
			break;
			
		case ACM_FULLCHECKER:
			print("Using all AC modules.\n");
			break;
	}
}