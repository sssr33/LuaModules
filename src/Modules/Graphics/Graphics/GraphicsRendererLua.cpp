#include "pch.h"
#include "GraphicsRendererLua.h"
#include "GraphicsRendererLuaInternal.h"
#include "GraphicsRenderer.h"

#include <memory>

static constexpr char *Graphics_GraphicsRendererMt = "Graphics.GraphicsRenderer";

int Graphics_GraphicsRenderer_delete(lua_State *L) {
    auto _this = Graphics_GraphicsRenderer_Internal_FromStack(L, 1);
    _this->~shared_ptr();
    return 0;
}

int Graphics_GraphicsRenderer_new(lua_State *L) {
    try {
        auto renderer = std::make_shared<GraphicsRenderer>();
        return Graphics_GraphicsRenderer_Internal_new(L, std::move(renderer));
    }
    catch (const std::exception &e) {
        std::string msg = "Failed to create GraphicsRenderer instance: ";
        msg += e.what();
        luaL_error(L, msg.c_str());
    }

    return 0;
}

int Graphics_GraphicsRenderer_Internal_new(lua_State *L, std::shared_ptr<GraphicsRenderer> renderer) {
    auto mem = lua_newuserdata(L, sizeof std::shared_ptr<GraphicsRenderer>);

    try {
        new(mem) std::shared_ptr<GraphicsRenderer>(std::move(renderer));
    }
    catch (const std::exception &e) {
        std::string msg = "Failed to create GraphicsRenderer instance: ";
        msg += e.what();
        luaL_error(L, msg.c_str());
    }

    luaL_getmetatable(L, Graphics_GraphicsRendererMt);
    lua_setmetatable(L, -2);

    return 1;
}

std::shared_ptr<GraphicsRenderer> *Graphics_GraphicsRenderer_Internal_FromStack(lua_State *L, int idx) {
    auto _this = (std::shared_ptr<GraphicsRenderer>*)luaL_checkudata(L, idx, Graphics_GraphicsRendererMt);
    return _this;
}

int Graphics_GraphicsRenderer_PropGet(lua_State *L) {
    auto top = lua_gettop(L);
    bool t = lua_istable(L, -2);
    bool s = lua_isstring(L, -1);

    auto tv = lua_topointer(L, -2);
    auto sv = lua_tostring(L, -1);

    luaL_getmetatable(L, Graphics_GraphicsRendererMt);
    auto tv2 = lua_topointer(L, -1);

    bool calledWithMt = tv == tv2;

    return 0;
}

int Graphics_GraphicsRenderer_PropSet(lua_State *L) {
    return 0;
}

void luaopen_Graphics_GraphicsRenderer(lua_State *L) {
    static const luaL_Reg funcs[] = {
        { "New", Graphics_GraphicsRenderer_new },
        { nullptr, nullptr }
    };
    /*static const luaL_Reg metaFuncs[] = {
        { "New", Graphics_RenderWindow_new },
        { nullptr, nullptr }
    };*/

    luaL_newlib(L, funcs);

    luaL_newmetatable(L, Graphics_GraphicsRendererMt);

    // set metatable as __index of itself
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");

    // set __gc method for metatable
    lua_pushcfunction(L, Graphics_GraphicsRenderer_delete);
    lua_setfield(L, -2, "__gc");

    // pop metatable to leave only library table at the top of the stack
    lua_pop(L, 1);
}