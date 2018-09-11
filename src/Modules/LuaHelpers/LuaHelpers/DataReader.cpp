#include "DataReader.h"

namespace LuaH {
    float DataReader::GetStackNumber(lua_State *L, int idx, const char *errorName, const float *defaultVal) {
        float num;
        auto type = lua_type(L, idx);

        if (type == LUA_TNUMBER) {
            num = (float)lua_tonumber(L, idx);
        }
        else if (defaultVal) {
            num = *defaultVal;
        }
        else {
            luaL_error(L, "Failed to load %s.", errorName);
        }

        return num;
    }

    float DataReader::GetStackNumber(lua_State *L, int idx, const char *errorName, const float &defaultVal) {
        return DataReader::GetStackNumber(L, idx, errorName, &defaultVal);
    }

    float DataReader::GetArrayNumber(lua_State *L, int idx, lua_Integer i, const char *errorName, const float *defaultVal) {
        float num;
        int itemType = lua_geti(L, idx, i);

        if (itemType == LUA_TNUMBER) {
            num = (float)lua_tonumber(L, -1);
        }
        else if (defaultVal) {
            num = *defaultVal;
        }
        else {
            luaL_error(L, "Failed to load %s.", errorName);
        }

        lua_pop(L, 1);
        return num;
    }

    float DataReader::GetArrayNumber(lua_State *L, int idx, lua_Integer i, const char *errorName, const float &defaultVal) {
        return DataReader::GetArrayNumber(L, idx, i, errorName, &defaultVal);
    }

    float DataReader::GetTableNumber(lua_State *L, int idx, const char *key, const char *errorName, const float *defaultVal) {
        float num;
        int itemType = lua_getfield(L, idx, key);

        if (itemType == LUA_TNUMBER) {
            num = (float)lua_tonumber(L, -1);
        }
        else if (defaultVal) {
            num = *defaultVal;
        }
        else {
            luaL_error(L, "Failed to load %s.", errorName);
        }

        lua_pop(L, 1);
        return num;
    }

    float DataReader::GetTableNumber(lua_State *L, int idx, const char *key, const char *errorName, const float &defaultVal) {
        return DataReader::GetTableNumber(L, idx, key, errorName, &defaultVal);
    }
}