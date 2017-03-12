#ifndef __LUA_CALLBACK_H__
#define __LUA_CALLBACK_H__

#ifdef _MSC_VER
#pragma once
#endif

#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <Psapi.h>
#include <Shellapi.h>
#include <lua.hpp>
#include "..\Logger\Logger.h"
#include "CVarSystem\CVarSystem.h"
#include "CVarSystem\CmdLineInterface.h"

namespace ScriptEngine
{
	class LuaCallback
	{
	private:
		std::vector< std::size_t >	m_vnCallbackErrorFunctions;
		std::vector< std::string >	m_vstrCallbackFunctions;
		
	private:
		std::size_t					GetCallbackFunctionIndex( std::string strFunctionName );
	public:
		LuaCallback( void );
		~LuaCallback( void );
		void						RegisterCallbackFunction( std::string strFunctionName );
		void						UnregisterCallbackFunction( std::string strFunctionName );
		void						CallRegisteredFunctions( lua_State *pLua );
	};
}

extern ScriptEngine::LuaCallback *g_luaEngine;

#endif