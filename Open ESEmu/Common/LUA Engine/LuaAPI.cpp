#include "LuaAPI.h"
#include "LuaInterface.h"

namespace ScriptEngine
{

	static bool lua_apicheck_args( lua_State *pLua, int iArgMin, int iArgMax, std::string strFunctionName )
	{
		auto iNumberOfArgs = lua_gettop( pLua );
		if( iNumberOfArgs > iArgMax || iNumberOfArgs < iArgMin )
		{
			LogWarning( "%s: %s arguments(%d/%d)!\n", strFunctionName.c_str( ), ( iNumberOfArgs > 2 ) ? "to many" : "missing", iNumberOfArgs, iArgMax );
			return false;
		}

		return true;
	}

	static int lua_api_bitband( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 2, 2, "BitBand" ) )
			return lua_ret_int( pLua, 0 );

		if( !lua_isinteger( pLua, 1 ) || !lua_isinteger( pLua, 2 ) )
		{
			LogWarning( "BitBand: one or more argument isn't an integer!" );
			return lua_ret_int( pLua, 0 );
		}

		return lua_ret_int( pLua, lua_to_int( pLua, 1 ) & lua_to_int( pLua, 2 ) );
	}

	static int lua_api_getkeystate( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 1, 1, "GetKeyState" ) )
			return lua_ret_int( pLua, 0 );

		if( !lua_is_digit( pLua, 1 ) )
		{
			LogWarning( "GetKeyState: argument isn't digit!" );
			return lua_ret_int( pLua, 0 );
		}

		return lua_ret_int( pLua, GetKeyState( lua_to_int( pLua, 1 ) ) );
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

	static int lua_api_include( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 1, 1, "Include" ) )
			return 0;

		if( !lua_isstring( pLua, 1 ) )
		{
			LogWarning( "Include: argument isn't a string!" );
			return 0;
		}

		if( luaL_loadfile( pLua, GetApplicationDirectory( lua_to_string( pLua, 1 ) ).c_str( ) ) || lua_pcall( pLua, 0, 0, 0 ) )
			LogWarning( "Include: error executing: %s!", luaL_checkstring( pLua, -1 ) );

		return 0;	
	}
	static int lua_api_sleep( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 1, 1, "Sleep" ) )
			return 0;

		if( !lua_is_digit( pLua, 1 ) )
		{
			LogWarning( "Sleep: argument isn't digit!" );
			return 0;
		}

		Sleep( ( DWORD )lua_to_int( pLua, 1 ) );
		return 0;
	}
	static int lua_api_callback_registercallbackfunction( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 1, 1, "LuaCallback::RegisterCallbackFunction" ) )
			return lua_ret_bool( pLua );

		if( !lua_isstring( pLua, 1 ) )
		{
			LogWarning( "LuaCallback::RegisterCallbackFunction: argument isn't a string!" );
			return lua_ret_bool( pLua );
		}

		g_luaEngine->RegisterCallbackFunction( lua_to_string( pLua, 1 ) );
		return lua_ret_bool( pLua, true );
	}

	static int lua_api_cvar_registercvar( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 2, 2, "CVarSystem::RegisterCVar" ) )
			return lua_ret_bool( pLua );

		if( !lua_isstring( pLua, 1 ) )
		{
			LogWarning( "CVarSystem::RegisterCVar: argument 1 isn't a string!" );
			return lua_ret_bool( pLua );
		}
		
		auto bDigit = lua_is_digit( pLua, 2 );
		auto bBoolean = ( lua_isboolean( pLua, 2 ) == 1 );

		if( !bDigit && !bBoolean )
		{
			LogWarning( "CVarSystem::RegisterCVar: argument 2 isn't a boolean or digit number!" );
			return lua_ret_bool( pLua );
		}

		auto flValue = 0.f;
		if( bDigit )
			flValue = ( float )lua_to_number( pLua, 2 );
		else flValue = ( lua_to_bool( pLua, 2 ) ? 1.f : 0.f );

		g_pCVarManager->RegisterCVar( lua_to_string( pLua, 1 ), flValue );
		return lua_ret_bool( pLua, true );
	}
	static int lua_api_cvar_getbool( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 1, 1, "CVarSystem::GetBool" ) )
			return lua_ret_bool( pLua );

		if( !lua_isstring( pLua, 1 ) )
		{
			LogWarning( "CVarSystem::GetBool: argument isn't a string!" );
			return lua_ret_bool( pLua );
		}

		return lua_ret_bool( pLua, g_pCVarManager->GetBool( lua_to_string( pLua, 1 ) ) );
	}
	static int lua_api_cvar_getfloat( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 1, 1, "CVarSystem::GetFloat" ) )
			return lua_ret_number( pLua );

		if( !lua_isstring( pLua, 1 ) )
		{
			LogWarning( "CVarSystem::GetFloat: argument isn't a string!" );
			return lua_ret_number( pLua );
		}

		return lua_ret_number( pLua, ( double )g_pCVarManager->GetFloat( lua_to_string( pLua, 1 ) ) );
	}

	static int lua_api_cvar_getint( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 1, 1, "CVarSystem::GetInt" ) )
			return lua_ret_int( pLua );

		if( !lua_isstring( pLua, 1 ) )
		{
			LogWarning( "CVarSystem::GetInt: argument isn't a string!" );
			return lua_ret_int( pLua );
		}

		return lua_ret_int( pLua, g_pCVarManager->GetInt( lua_to_string( pLua, 1 ) ) );
	}

	static int lua_api_cvar_setvalue( lua_State *pLua )
	{
		if( !lua_apicheck_args( pLua, 2, 2, "CVarSystem::SetValue" ) )
			return lua_ret_bool( pLua );

		if( !lua_isstring( pLua, 1 ) )
		{
			LogWarning( "CVarSystem::RegisterCVar: argument 1 isn't a string!" );
			return lua_ret_bool( pLua );
		}

		auto bDigit = lua_is_digit( pLua, 2 );
		auto bBoolean = ( lua_isboolean( pLua, 2 ) == 1 );

		if( !bDigit && !bBoolean )
		{
			LogWarning( "CVarSystem::RegisterCVar: argument 2 isn't a boolean or digit number!" );
			return lua_ret_bool( pLua );
		}

		auto flValue = 0.f;
		if( bDigit )
			flValue = ( float )lua_to_number( pLua, 2 );
		else flValue = ( lua_to_bool( pLua, 2 ) ? 1.f : 0.f );

		g_pCVarManager->SetValue( lua_to_string( pLua, 1 ), flValue );
		return lua_ret_bool( pLua, true );
	}
	
	
	
	void lua_api_register( lua_State *pLua )
	{
		static luaL_Reg CVarSystemLibrary[] =
		{
			{ "RegisterCVar", lua_api_cvar_registercvar },
			{ "GetBool", lua_api_cvar_getbool },
			{ "GetFloat", lua_api_cvar_getfloat },
			{ "GetInt", lua_api_cvar_getint },
			{ "SetValue", lua_api_cvar_setvalue },
			{ NULL, NULL }
		};

		lua_export_function( pLua, "RegisterCallbackFunction", lua_api_callback_registercallbackfunction );
		lua_export_function( pLua, "BitBand", lua_api_bitband );
		lua_export_function( pLua, "GetKeyState", lua_api_getkeystate );
		lua_export_function( pLua, "Include", lua_api_include );
		lua_export_function( pLua, "Sleep", lua_api_sleep );
		lua_export_library( pLua, "CVar", CVarSystemLibrary );
	}
}