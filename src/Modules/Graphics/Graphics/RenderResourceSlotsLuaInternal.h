#pragma once
#include "Render/RenderResourceSlots.h"

#include <LuaHelpers/luaInc.h>

namespace Lua {
    namespace Internal {
        struct RenderResourceSlotsLua {
            typedef RenderResourceSlots CType;

            static CType *GetFromStack(lua_State *L, int idx);
            static CType *TryGetFromStack(lua_State *L, int idx);
        };
    }
}