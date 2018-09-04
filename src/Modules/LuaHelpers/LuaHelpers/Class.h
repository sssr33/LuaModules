#pragma once
#include "luaInc.h"

namespace LuaH {
    class Class {
    public:
        // Registers C/C++ class as lua userdata leaving <classFn> table on stack
        template<size_t FnSz, size_t MtFnSz>
        static void Register(
            lua_State *L,
            const luaL_Reg(&classFn)[FnSz],
            const luaL_Reg(&classMtFn)[MtFnSz],
            const char *classMtName
        )
        {
            luaL_checkversion(L);

            luaL_newlibtable(L, classFn);
            luaL_setfuncs(L, classFn, 0);

            if (luaL_newmetatable(L, classMtName)) {
                // set functions only when new table was created
                luaL_setfuncs(L, classMtFn, 0);
            }

            lua_pop(L, 1); // pop metatable to leave only library table at the top of the stack
        }
    };
}