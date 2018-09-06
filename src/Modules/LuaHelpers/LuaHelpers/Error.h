#pragma once
#include "luaInc.h"

#include <stdexcept>

namespace LuaH {
    class Error {
    public:
        static void FailedToCreate(lua_State *L, const char *className);
        static void FailedToCreate(lua_State *L, const char *className, const std::exception &ex);
        static void FailedToCreate(lua_State *L, const char *className, const char *description);

        // Intended to be used by lua class's PropertySetter where index of property key is always 2nd argument.
        static void FailedPropertySetter(lua_State *L, const char *className, int propKeyIdx = 2);
    };
}