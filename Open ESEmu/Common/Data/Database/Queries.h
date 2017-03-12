#pragma once
#ifndef _QUERIES_H_
#define _QUERIES_H_

#define QUERY_SELECT_ACCOUNT_LOGIN "SELECT Accounts.* FROM Accounts WHERE Accounts.Email = '%s' AND Accounts.Password = '%s';"
#define QUERY_SELECT_BAN_EXPIRE	   "SELECT Accounts.BannedUntil FROM Accounts WHERE Accounts.ID = '%d';"
#define QUERY_SELECT_BASIC_AUTH	   "SELECT APIAuths.ID FROM APIAuths WHERE APIAuths.`User` = '%s' AND APIAuths.`Password` = '%s';"
#define QUERY_SELECT_TOKEN_AUTH	   "SELECT APIAuths.ID FROM APIAuths WHERE APIAuths.`User` = '%s' AND APIAuths.`Token` = '%s';"



#endif