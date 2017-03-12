#pragma once
#ifndef _exports_H_
#define _exports_H_

#include <lua.hpp>
#include <math.h>
#include "..\..\Common\Logger\Logger.h"

static int sigmoid(lua_State* _vm)
{
	if (lua_gettop(_vm) == 1)
	{
		double _sigmoidy = 2 / (1 + exp(-4.9 * ScriptEngine::lua_to_number(_vm, 1))) - 1;
		ScriptEngine::lua_push_number(_vm, _sigmoidy);
	} else
		return luaL_error(_vm, "Sigmoid requires 1 argument, you parsed %d", lua_gettop(_vm));
	return 1;
}

static int notifyError(lua_State* _vm)
{
	if (lua_gettop(_vm) == 1)
	{
		LogErrorLUA("%s", ScriptEngine::lua_to_string(_vm, 1).c_str());
	} else
		return luaL_error(_vm, "notifyError requires 1 argument, you parsed %d", lua_gettop(_vm));
	return 1;
}

static int _registerNeuralFunctionalities(lua_State* _vm)
{
	ScriptEngine::lua_export_function(_vm, "sigmoid", sigmoid);
	return 1;
}

static int _registerLuaErrorReporting(lua_State* _vm)
{
	ScriptEngine::lua_export_function(_vm, "printError", notifyError);
	return 1;
}

#endif