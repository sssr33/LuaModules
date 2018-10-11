#pragma once
#include "luaInc.h"

#include <cstdint>

namespace LuaH {
    class DataReader {
        template<class T> struct GetStackHelper {
            static int GetType(lua_State *L, int idx) {
                static_assert(false, "No impl");
            }

            static T ToType(lua_State *L, int idx, const char *errorName) {
                static_assert(false, "No impl");
            }
        };
    public:
        template<class T> static T GetStack(lua_State *L, int idx, const char *errorName, const T &defaultVal, bool *defaultValueUsed = nullptr) {
            return DataReader::GetStack<T>(L, idx, errorName, &defaultVal, defaultValueUsed);
        }

        template<class T> static T GetStack(lua_State *L, int idx, const char *errorName, const T *defaultVal = nullptr, bool *defaultValueUsed = nullptr) {
            auto type = lua_type(L, idx);
            return DataReader::GetStack<T>(L, type, idx, errorName, defaultVal, defaultValueUsed);
        }

        template<class T> static T GetArray(lua_State *L, int idx, lua_Integer i, const char *errorName, const T &defaultVal, bool *defaultValueUsed = nullptr) {
            return DataReader::GetArray<T>(L, idx, i, errorName, &defaultVal, defaultValueUsed);
        }

        template<class T> static T GetArray(lua_State *L, int idx, lua_Integer i, const char *errorName, const T *defaultVal = nullptr, bool *defaultValueUsed = nullptr) {
            auto type = lua_geti(L, idx, i);
            auto val = DataReader::GetStack<T>(L, type, -1, errorName, defaultVal, defaultValueUsed);
            lua_pop(L, 1);
            return val;
        }

        template<class T> static T GetTable(lua_State *L, int idx, const char *key, const char *errorName, const T &defaultVal, bool *defaultValueUsed = nullptr) {
            return DataReader::GetTable<T>(L, idx, key, errorName, &defaultVal, defaultValueUsed);
        }

        template<class T> static T GetTable(lua_State *L, int idx, const char *key, const char *errorName, const T *defaultVal = nullptr, bool *defaultValueUsed = nullptr) {
            auto type = lua_getfield(L, idx, key);
            auto val = DataReader::GetStack<T>(L, type, -1, errorName, defaultVal, defaultValueUsed);
            lua_pop(L, 1);
            return val;
        }

        template<class T> static T GetStack(lua_State *L, int luaType, int idx, const char *errorName, const T *defaultVal = nullptr, bool *defaultValueUsed = nullptr) {
            T val;

            if (luaType == GetStackHelper<T>::GetType(L, idx)) {
                val = GetStackHelper<T>::ToType(L, idx, errorName);
            }
            else if (defaultVal) {
                val = *defaultVal;

                if (defaultValueUsed) {
                    *defaultValueUsed = true;
                }
            }
            else {
                luaL_error(L, "Failed to load %s.", errorName);
            }

            return val;
        }

    private:
        template<> struct GetStackHelper<float> {
            static int GetType(lua_State *L, int idx);
            static float ToType(lua_State *L, int idx, const char *errorName);
        };

        template<> struct GetStackHelper<uint32_t> {
            static int GetType(lua_State *L, int idx);
            static uint32_t ToType(lua_State *L, int idx, const char *errorName);
        };

        template<> struct GetStackHelper<bool> {
            static int GetType(lua_State *L, int idx);
            static bool ToType(lua_State *L, int idx, const char *errorName);
        };
    };
}