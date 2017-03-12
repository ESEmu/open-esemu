#pragma once
#include "__opcImporter.h"

class SessionHandler;
class StreamHandler;


#define OPCODE(P1, P2) { case P1: P2(pr, sk); break; }

// LOGIN SERVER -----------------------------------
#define OPCODE_REQ_AUTH			0x6500
#define OPCODE_REQ_UNK1			0x7600
#define OPCODE_REQ_NAMEAVAIL	0x7800
#define OPCODE_REQ_CCHARPHASE	0x7900
#define OPCODE_REQ_LANGSERVER	0xF900
#define OPCODE_REQ_AREYOUALIVE	

// GAME SERVER ------------------------------------

// ------------------------------------------------

bool DeterminatePacket(StreamHandler* pr, SessionHandler* sk)
{
	unsigned short OPCode = NULL;
	pr->r->Skip(sizeof(unsigned short));
	pr->r->ReadShort(OPCode);

	switch (OPCode)
	{
		//OPCODE(OPCODE_REQ_AUTH,			REQ_AUTH);
		//OPCODE(OPCODE_REQ_UNK1,			REQ_UNK1);
		//OPCODE(OPCODE_REQ_NAMEAVAIL,	REQ_NAMEAVAIL);
		//OPCODE(OPCODE_REQ_CCHARPHASE, 	REQ_CCHARPHASE);
		//OPCODE(OPCODE_REQ_LANGSERVER,	REQ_SELECTLANG);

	default:
		
		printf("[!] Packet not recognized (%02X)!\n", OPCode);
		for (int i = 0; i < pr->r->GetSize(); i++)
			printf("%02X ", pr->r->GetBuffer()[i]);
		return false;
	}
	return true;
}
