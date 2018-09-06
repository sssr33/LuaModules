#include "pch.h"
#include "Graphics.h"
#include "RenderWindowLua.h"
#include "GraphicsRendererLua.h"
#include "RenderCmdListLua.h"
#include "RenderResourceSlotsLua.h"

int luaopen_Graphics(lua_State *L) {
    luaL_checkversion(L);

    lua_createtable(L, 0, 0);

    Lua::LuaOpenRenderWindow(L);
    lua_setfield(L, -2, "RenderWindow");

    Lua::LuaOpenGraphicsRenderer(L);
    lua_setfield(L, -2, "GraphicsRenderer");

    Lua::LuaOpenRenderCmdList(L);
    lua_setfield(L, -2, "RenderCmdList");

    Lua::LuaOpenRenderResourceSlots(L);
    lua_setfield(L, -2, "RenderResourceSlots");

    return 1;
}