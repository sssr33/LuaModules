#pragma once
#include "luaInc.h"

#include <string.h>

namespace LuaH {
    class Class {
    public:
        // Registers C/C++ class as lua userdata leaving <classFn> table on stack
        // <metatableAsIndex> - if true sets <classMtFn> as its __index
        template<size_t FnSz, size_t MtFnSz>
        static void Register(
            lua_State *L,
            const luaL_Reg(&classFn)[FnSz],
            const luaL_Reg(&classMtFn)[MtFnSz],
            const char *classMtName,
            bool metatableAsIndex = false
        )
        {
            luaL_checkversion(L);

            luaL_newlibtable(L, classFn);
            luaL_setfuncs(L, classFn, 0);

            if (luaL_newmetatable(L, classMtName)) {
                // set functions only when new table was created and not when existing was returned
                luaL_setfuncs(L, classMtFn, 0);

                if (metatableAsIndex) {
                    lua_pushstring(L, "__index");
                    lua_pushvalue(L, -2); // push copy of mt
                    lua_rawset(L, -3); // set mt.__index = mt to allow lua access its methods
                }
            }

            lua_pop(L, 1); // pop metatable to leave only library table at the top of the stack
        }
    };
}