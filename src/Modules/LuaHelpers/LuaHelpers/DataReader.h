#pragma once
#include "luaInc.h"

namespace LuaH {
    class DataReader {
    public:
        static float GetStackNumber(lua_State *L, int idx, const char *errorName, const float *defaultVal = nullptr);
        static float GetStackNumber(lua_State *L, int idx, const char *errorName, const float &defaultVal);

        static float GetArrayNumber(lua_State *L, int idx, lua_Integer i, const char *errorName, const float *defaultVal = nullptr);
        static float GetArrayNumber(lua_State *L, int idx, lua_Integer i, const char *errorName, const float &defaultVal);

        static float GetTableNumber(lua_State *L, int idx, const char *key, const char *errorName, const float *defaultVal = nullptr);
        static float GetTableNumber(lua_State *L, int idx, const char *key, const char *errorName, const float &defaultVal);
    };
}