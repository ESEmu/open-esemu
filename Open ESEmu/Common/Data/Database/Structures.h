#pragma once
#ifndef _STRUCTURES_H_
#define _STRUCTURE_H_

#include <iostream>
#include <string>
#include <mysql.h>
using namespace std;

/*
https://dev.mysql.com/doc/refman/5.7/en/c-api-prepared-statement-type-codes.html

	C Type		  |	MySQL Header Type	  |	SQL Parser Type
-----------------------------------------------------------------------
	signed char		MYSQL_TYPE_TINY			TINYINT
	short int		MYSQL_TYPE_SHORT		SMALLINT
	int				MYSQL_TYPE_LONG			INT
	long long int	MYSQL_TYPE_LONGLONG		BIGINT
	float			MYSQL_TYPE_FLOAT		FLOAT
	double			MYSQL_TYPE_DOUBLE		DOUBLE
	MYSQL_TIME		MYSQL_TYPE_TIME			TIME
	MYSQL_TIME		MYSQL_TYPE_DATE			DATE
	MYSQL_TIME		MYSQL_TYPE_DATETIME		DATETIME
	MYSQL_TIME		MYSQL_TYPE_TIMESTAMP	TIMESTAMP
	char[]			MYSQL_TYPE_STRING		TEXT, CHAR, VARCHAR
	char[]			MYSQL_TYPE_BLOB			BLOB, BINARY, VARBINARY
					MYSQL_TYPE_NULL			NULL
-----------------------------------------------------------------------

*/
typedef struct _USER_INFO
{
	unsigned int _id;
	char*		 _email;
	char*		 _password; // WILL NOT BE STORED IN MEMORY, IF USED MUST BE MEMSETTED!
	MYSQL_TIME	 _joinDate;
	char*		 _status;
	char*		 _authLevel;
	MYSQL_TIME	 _lastLogin;
	char*		 _securityPIN;
	MYSQL_TIME	 _bannedUntil;
} USERAUTH_INFO;

#endif