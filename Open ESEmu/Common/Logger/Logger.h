#pragma once
#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <string>
using namespace std;
#include "..\Console Colors\advconsole.h"
#pragma warning (disable: 4996)

#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <Windows.h>
#include <strsafe.h>
using namespace std;
using namespace AdvancedConsole;

inline static void Log(const char* lpszFormat, ...)
{
	va_list va;
	va_start(va, lpszFormat);

	char lpszBuffer[256];
	unsigned long ulLength = vsprintf(lpszBuffer, lpszFormat, va);

	cout << Color(AC_WHITE) << "[" << Color(AC_GREEN) << "+" << Color(AC_WHITE) << "] " << lpszBuffer << "\n";

	va_end(va);
}

inline static void LogWarning(const char* lpszFormat, ...)
{
	va_list va;
	va_start(va, lpszFormat);

	char lpszBuffer[256];
	unsigned long ulLength = vsprintf(lpszBuffer, lpszFormat, va);

	cout << Color(AC_WHITE) << "[" << Color(AC_YELLOW) << "*" << Color(AC_WHITE) << "] " << lpszBuffer << "\n";

	va_end(va);
}

inline static void LogError(const char* lpszFormat, ...)
{
	va_list va;
	va_start(va, lpszFormat);

	char lpszBuffer[256];
	unsigned long ulLength = vsprintf(lpszBuffer, lpszFormat, va);
	cout << Color(AC_WHITE) << "[" << Color(AC_RED) << "!" << Color(AC_WHITE) << "] " << lpszBuffer << "\n";

	va_end(va);
}

inline static void LogErrorLUA(const char* lpszFormat, ...)
{
	va_list va;
	va_start(va, lpszFormat);

	char lpszBuffer[256];
	unsigned long ulLength = vsprintf(lpszBuffer, lpszFormat, va);
	cout << Color(AC_WHITE) << "[" << Color(AC_RED) << "LUA!" << Color(AC_WHITE) << "] " << Color(AC_YELLOW) << lpszBuffer << Color(AC_WHITE) << "\n";

	va_end(va);
}

#endif