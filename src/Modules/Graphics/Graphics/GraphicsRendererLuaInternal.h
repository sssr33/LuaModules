#pragma once
#include "GraphicsRenderer.h"

#include <memory>
#include <LuaHelpers/luaInc.h>

namespace Lua {
    namespace Internal {
        struct GraphicsRendererLua {
            static int Create(lua_State *L, std::shared_ptr<GraphicsRenderer> renderer);
            static std::shared_ptr<GraphicsRenderer> *GetFromStack(lua_State *L, int idx);
        };
    }
}