#include "pch.h"
#include "DataReaderLua.h"

#include <LuaHelpers/DataReader.h>

namespace Lua {
    Color DataReader::GetColor(lua_State *L, int idx) {
        constexpr char *ErrorRed = "Color.red";
        constexpr char *ErrorGreen = "Color.green";
        constexpr char *ErrorBlue = "Color.blue";
        constexpr char *ErrorAlpha = "Color.alpha";

        Color color;
        idx = lua_absindex(L, idx);
        auto type = lua_type(L, idx);

        switch (type) {
        case LUA_TNUMBER: {
            color.r = LuaH::DataReader::GetStackNumber(L, idx, ErrorRed);
            color.g = LuaH::DataReader::GetStackNumber(L, idx + 1, ErrorGreen);
            color.b = LuaH::DataReader::GetStackNumber(L, idx + 2, ErrorBlue);
            color.a = LuaH::DataReader::GetStackNumber(L, idx + 3, ErrorAlpha, 1.f);
            break;
        }
        case LUA_TTABLE: {
            lua_Integer len = luaL_len(L, idx);

            if (len >= 3) {
                color.r = LuaH::DataReader::GetArrayNumber(L, idx, 1, ErrorRed);
                color.g = LuaH::DataReader::GetArrayNumber(L, idx, 2, ErrorGreen);
                color.b = LuaH::DataReader::GetArrayNumber(L, idx, 3, ErrorBlue);
                color.a = LuaH::DataReader::GetArrayNumber(L, idx, 4, ErrorAlpha, 1.f);
            }
            else {
                color.r = LuaH::DataReader::GetTableNumber(L, idx, "r", ErrorRed);
                color.g = LuaH::DataReader::GetTableNumber(L, idx, "g", ErrorGreen);
                color.b = LuaH::DataReader::GetTableNumber(L, idx, "b", ErrorBlue);
                color.a = LuaH::DataReader::GetTableNumber(L, idx, "a", ErrorAlpha, 1.f);
            }
            break;
        }
        default:
            luaL_error(L, "Unexpected type when parsing Color. Got %s.", lua_typename(L, type));
            break;
        }

        return color;
    }

    Rect DataReader::GetRect(lua_State *L, int idx) {
        constexpr char *ErrorLeft = "Rect.left";
        constexpr char *ErrorTop = "Rect.top";
        constexpr char *ErrorRight = "Rect.right";
        constexpr char *ErrorBottom = "Rect.bottom";

        Rect rect;
        idx = lua_absindex(L, idx);
        auto type = lua_type(L, idx);

        switch (type) {
        case LUA_TNUMBER: {
            rect.left = LuaH::DataReader::GetStackNumber(L, idx, ErrorLeft);
            rect.top = LuaH::DataReader::GetStackNumber(L, idx + 1, ErrorTop);
            rect.right = LuaH::DataReader::GetStackNumber(L, idx + 2, ErrorRight);
            rect.bottom = LuaH::DataReader::GetStackNumber(L, idx + 3, ErrorBottom);
            break;
        }
        case LUA_TTABLE: {
            lua_Integer len = luaL_len(L, idx);

            if (len >= 4) {
                rect.left = LuaH::DataReader::GetArrayNumber(L, idx, 1, ErrorLeft);
                rect.top = LuaH::DataReader::GetArrayNumber(L, idx, 2, ErrorTop);
                rect.right = LuaH::DataReader::GetArrayNumber(L, idx, 3, ErrorRight);
                rect.bottom = LuaH::DataReader::GetArrayNumber(L, idx, 4, ErrorBottom);
            }
            else {
                rect.left = LuaH::DataReader::GetTableNumber(L, idx, "left", ErrorLeft);
                rect.top = LuaH::DataReader::GetTableNumber(L, idx, "top", ErrorTop);
                rect.right = LuaH::DataReader::GetTableNumber(L, idx, "right", ErrorRight);
                rect.bottom = LuaH::DataReader::GetTableNumber(L, idx, "bottom", ErrorBottom);
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