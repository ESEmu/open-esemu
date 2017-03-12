#pragma once
#ifndef _SESSIONCACHE_H_
#define _SESSIONCACHE_H_

#include <iostream>
#include <string>
using namespace std;

enum ePlayerGrade
{
	GRADE_NORMAL = 10,
	GRADE_LEAGUE = 20,
	GRADE_SPECTATOR_LV1 = 25,
	GRADE_SPECTATOR_LV2 = 26,
	GRADE_SPECTATOR_LV3 = 27,
	GRADE_QUALITYASSURANCE = 30,
	GRADE_GAMEMASTER = 40,
	GRADE_DEVELOPER = 50
};

class _sessionCache
{
public:	

	struct _USER_INFO
	{
		string			_userNickname = "";
		string			_userEmail = "";
		unsigned int	_userID = -1;
		unsigned int	_userCoins = 0;
		unsigned int	_userRealCoins = 0;
		unsigned short	_playerGrade = GRADE_NORMAL;
	} USER_INFO;

};

#endif