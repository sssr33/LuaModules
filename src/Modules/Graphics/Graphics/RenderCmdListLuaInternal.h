#pragma once
#include "Render/RenderCmdList.h"

#include <LuaHelpers/luaInc.h>

namespace Lua {
    namespace Internal {
        struct RenderCmdListLua {
            typedef RenderCmdList CType;

            static CType *GetFromStack(lua_State *L, int idx);
        };
    }
}