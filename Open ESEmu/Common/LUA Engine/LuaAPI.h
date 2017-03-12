#ifndef __LUA_LUAAPI_H__
#define __LUA_LUAAPI_H__

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
#include "Callback.h"
#include "CVarSystem\CVarSystem.h"
#include "CVarSystem\CmdLineInterface.h"

namespace ScriptEngine
{
	extern void lua_api_register( lua_State *pLua );
}



#endif