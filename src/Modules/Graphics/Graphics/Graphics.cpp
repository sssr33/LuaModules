#include "pch.h"
#include "Graphics.h"
#include "RenderWindowLua.h"
#include "GraphicsRendererLua.h"
#include "RenderCmdListLua.h"
#include "RenderResourceSlotsLua.h"

int luaopen_Graphics(lua_State *L) {
    luaL_checkversion(L);

    lua_createtable(L, 0, 0);

    luaopen_Graphics_RenderWindow(L);
    lua_setfield(L, -2, "RenderWindow");

    luaopen_Graphics_GraphicsRenderer(L);
    lua_setfield(L, -2, "GraphicsRenderer");

    luaopen_Graphics_RenderCmdList(L);
    lua_setfield(L, -2, "RenderCmdList");

    luaopen_Graphics_RenderResourceSlots(L);
    lua_setfield(L, -2, "RenderResourceSlots");

    return 1;
}