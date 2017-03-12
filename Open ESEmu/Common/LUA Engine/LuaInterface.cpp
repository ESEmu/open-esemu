#include "LuaInterface.h"

#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <Psapi.h>
#include <Shellapi.h>
#include <lua.hpp>
#include "..\Logger\Logger.h"

namespace ScriptEngine
{
	bool lua_is_digit( lua_State *pLua, int iStackPos )
	{
		return( lua_isinteger( pLua, iStackPos ) || lua_isnumber( pLua, iStackPos ) );
	}

	bool lua_to_bool( lua_State *pLua, int iStackPos )
	{
		auto iRetVal = lua_toboolean( pLua, iStackPos );
		return ( iRetVal >= 1 );
	}

	double lua_to_number( lua_State *pLua, int iStackPos )
	{
		return lua_tonumber( pLua, iStackPos );
	}

	int lua_to_int( lua_State *pLua, int iStackPos )
	{
		return ( int )lua_tointeger( pLua, iStackPos );
	}

	int lua_ret_bool( lua_State *pLua, bool bRetVal )
	{
		lua_push_bool( pLua, bRetVal );
		return 1;
	}

	int lua_ret_int( lua_State *pLua, int iRetVal )
	{
		lua_push_int( pLua, iRetVal );
		return 1;
	}

	int lua_ret_number( lua_State *pLua, double nRetVal )
	{
		lua_push_number( pLua, nRetVal );
		return 1;
	}

	int lua_ret_nil( lua_State *pLua )
	{
		lua_push_nil( pLua );
		return 1;
	}

	int lua_ret_string( lua_State *pLua, std::string strRetVal )
	{
		lua_push_string( pLua, strRetVal );
		return 1;
	}

	lua_State *lua_attach( void )
	{
		lua_State *pLua = luaL_newstate( );
		luaL_openlibs( pLua );
		luaopen_math(pLua);
		return pLua;
	}

	std::string lua_to_string( lua_State* pLua, int iStackPos )
	{
		return std::string( lua_tostring( pLua, iStackPos ) );
	}

	inline std::string GetApplicationDirectory(std::string strOptionalFileInDir)
	{
		char pszPath[MAX_PATH];
		GetCurrentDirectoryA(MAX_PATH, pszPath);

		auto strApplicationDirectory = std::string(pszPath) + std::string("\\");
		if (!strOptionalFileInDir.empty())
			strApplicationDirectory.append(strOptionalFileInDir);

		return strApplicationDirectory;
	}

	void lua_execute( lua_State *pLua, std::string strLuaFile )
	{
		if( strLuaFile.empty( ) )
		{
			LogWarning("lua_execute: empty lua file!");
			return;
		}

		if( luaL_dofile( pLua, GetApplicationDirectory( strLuaFile ).c_str( ) ) )
		{
			LogWarning( "lua_execute: %s!", lua_tostring( pLua, -1 ) );
		}
	}

	void lua_release( lua_State *pLua )
	{
		lua_close( pLua );
	}

	void lua_push_bool( lua_State *pLua, bool bRetVal )
	{
		lua_pushboolean( pLua, bRetVal ? 1 : 0 );
	}

	void lua_push_int( lua_State *pLua, int iRetVal )
	{
		lua_pushinteger( pLua, iRetVal );
	}

	void lua_push_number( lua_State *pLua, double nRetVal )
	{
		lua_pushnumber( pLua, nRetVal );
	}

	void lua_push_nil( lua_State *pLua )
	{
		lua_pushnil( pLua );
	}

	void lua_push_string( lua_State *pLua, std::string strRetVal )
	{
		lua_pushstring( pLua, strRetVal.c_str( ) );
	}

	void lua_export_function( lua_State *pLua, std::string strFunctionName, const lua_CFunction fn )
	{
		if( strFunctionName.empty( ) )
		{
			LogWarning( "lua_export_function: empty function name!" );
			return;
		}

		lua_register( pLua, strFunctionName.c_str( ), fn );
	}
	void lua_export_library( lua_State *pLua, std::string strLibraryName, const luaL_Reg *pLibraryFunctions )
	{
		if( strLibraryName.empty( ) )
		{
			LogWarning( "lua_export_library: empty library name!" );
			return;
		}

		lua_newtable( pLua );
		lua_pushvalue( pLua, -1 );
		lua_setglobal( pLua, strLibraryName.c_str( ) );
		luaL_setfuncs( pLua, pLibraryFunctions, 0 );
	}
}