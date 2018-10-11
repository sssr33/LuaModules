#include "pch.h"
#include "DataReaderLua.h"

#include <LuaHelpers/DataReader.h>

namespace Lua {
    Color DataReader::GetColor(lua_State *L, int idx, int *readStackItems) {
        constexpr char *ErrorRed = "Color.red";
        constexpr char *ErrorGreen = "Color.green";
        constexpr char *ErrorBlue = "Color.blue";
        constexpr char *ErrorAlpha = "Color.alpha";

        Color color;
        idx = lua_absindex(L, idx);
        auto type = lua_type(L, idx);

        switch (type) {
        case LUA_TNUMBER: {
            color.r = LuaH::DataReader::GetStack<float>(L, idx, ErrorRed);
            color.g = LuaH::DataReader::GetStack<float>(L, idx + 1, ErrorGreen);
            color.b = LuaH::DataReader::GetStack<float>(L, idx + 2, ErrorBlue);

            if (readStackItems) {
                bool defaultUsed = false;

                color.a = LuaH::DataReader::GetStack<float>(L, idx + 3, ErrorAlpha, 1.f, &defaultUsed);
                *readStackItems = 4;

                if (defaultUsed) {
                    *readStackItems--;
                }
            }
            else {
                color.a = LuaH::DataReader::GetStack<float>(L, idx + 3, ErrorAlpha, 1.f);
            }

            break;
        }
        case LUA_TTABLE: {
            lua_Integer len = luaL_len(L, idx);

            if (len >= 3) {
                color.r = LuaH::DataReader::GetArray<float>(L, idx, 1, ErrorRed);
                color.g = LuaH::DataReader::GetArray<float>(L, idx, 2, ErrorGreen);
                color.b = LuaH::DataReader::GetArray<float>(L, idx, 3, ErrorBlue);
                color.a = LuaH::DataReader::GetArray<float>(L, idx, 4, ErrorAlpha, 1.f);
            }
            else {
                color.r = LuaH::DataReader::GetTable<float>(L, idx, "r", ErrorRed);
                color.g = LuaH::DataReader::GetTable<float>(L, idx, "g", ErrorGreen);
                color.b = LuaH::DataReader::GetTable<float>(L, idx, "b", ErrorBlue);
                color.a = LuaH::DataReader::GetTable<float>(L, idx, "a", ErrorAlpha, 1.f);
            }

            if (readStackItems) {
                *readStackItems = 1;
            }
            break;
        }
        default:
            luaL_error(L, "Unexpected type when parsing Color. Got %s.", lua_typename(L, type));
            break;
        }

        return color;
    }

    Rect DataReader::GetRect(lua_State *L, int idx, int *readStackItems) {
        constexpr char *ErrorLeft = "Rect.left";
        constexpr char *ErrorTop = "Rect.top";
        constexpr char *ErrorRight = "Rect.right";
        constexpr char *ErrorBottom = "Rect.bottom";

        Rect rect;
        idx = lua_absindex(L, idx);
        auto type = lua_type(L, idx);

        switch (type) {
        case LUA_TNUMBER: {
            rect.left = LuaH::DataReader::GetStack<float>(L, idx, ErrorLeft);
            rect.top = LuaH::DataReader::GetStack<float>(L, idx + 1, ErrorTop);
            rect.right = LuaH::DataReader::GetStack<float>(L, idx + 2, ErrorRight);
            rect.bottom = LuaH::DataReader::GetStack<float>(L, idx + 3, ErrorBottom);

            if (readStackItems) {
                *readStackItems = 4;
            }
            break;
        }
        case LUA_TTABLE: {
            lua_Integer len = luaL_len(L, idx);

            if (len >= 4) {
                rect.left = LuaH::DataReader::GetArray<float>(L, idx, 1, ErrorLeft);
                rect.top = LuaH::DataReader::GetArray<float>(L, idx, 2, ErrorTop);
                rect.right = LuaH::DataReader::GetArray<float>(L, idx, 3, ErrorRight);
                rect.bottom = LuaH::DataReader::GetArray<float>(L, idx, 4, ErrorBottom);
            }
            else {
                rect.left = LuaH::DataReader::GetTable<float>(L, idx, "left", ErrorLeft);
                rect.top = LuaH::DataReader::GetTable<float>(L, idx, "top", ErrorTop);
                rect.right = LuaH::DataReader::GetTable<float>(L, idx, "right", ErrorRight);
                rect.bottom = LuaH::DataReader::GetTable<float>(L, idx, "bottom", ErrorBottom);
            }

            if (readStackItems) {
                *readStackItems = 1;
            }
            break;
        }
        default:
            luaL_error(L, "Unexpected type when parsing Rect. Got %s.", lua_typename(L, type));
            break;
        }

        return rect;
    }
}