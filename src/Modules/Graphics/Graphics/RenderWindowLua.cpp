#include "pch.h"
#include "RenderWindowLua.h"
#include "RenderWindow.h"
#include "GraphicsRendererLuaInternal.h"

#include <libhelpers/UtfConvert.h>
#include <libhelpers/fnv1.h>

static constexpr char *Graphics_RenderWindowMt = "Graphics.RenderWindow";
static constexpr char *Graphics_RenderWindow_PropRenderer = "Renderer";

int Graphics_RenderWindow_delete(lua_State *L) {
    auto _this = reinterpret_cast<RenderWindow*>(luaL_checkudata(L, 1, Graphics_RenderWindowMt));

    // remove private instance table from private class table
    lua_pushlightuserdata(L, _this);
    lua_pushnil(L);
    lua_rawset(L, lua_upvalueindex(1));

    _this->~RenderWindow();
    return 0;
}

int Graphics_RenderWindow_new(lua_State *L) {
    std::wstring title = L"RenderWindow";

    if (lua_isstring(L, 1)) {
        title = ConvertToUtf16(lua_tostring(L, 1));
    }

    auto mem = lua_newuserdata(L, sizeof RenderWindow);

    try {
        new(mem) RenderWindow(std::move(title));
    }
    catch (const std::exception &e) {
        std::string msg = "Failed to create RenderWindow instance: ";
        msg += e.what();
        luaL_error(L, msg.c_str());
    }

    luaL_getmetatable(L, Graphics_RenderWindowMt);
    lua_setmetatable(L, -2);

    // set private instance table in private class table
    lua_pushlightuserdata(L, mem);
    lua_newtable(L);

    // fill private instance table
    {
        auto obj = reinterpret_cast<RenderWindow*>(mem);

        auto renderer = obj->GetRenderer();
        Graphics_GraphicsRenderer_Internal_new(L, std::move(renderer));
        lua_setfield(L, -2, Graphics_RenderWindow_PropRenderer);
    }

    lua_rawset(L, lua_upvalueindex(1));

    return 1;
}

int Graphics_RenderWindow_PropGet(lua_State *L) {
    auto _this = reinterpret_cast<RenderWindow*>(luaL_checkudata(L, 1, Graphics_RenderWindowMt));
    auto key = hash::fnv1<uint32_t>::hash(luaL_checkstring(L, 2));

    // push private instance table
    lua_pushlightuserdata(L, _this);
    lua_rawget(L, lua_upvalueindex(1));

    // check key
    switch (key) {
    case hash::fnv1<uint32_t>::hash(Graphics_RenderWindow_PropRenderer): {
        lua_pushstring(L, lua_tostring(L, 2));
        break;
    }
    default:
        luaL_error(L, "Can't access Graphics.RenderWindow.%s", lua_tostring(L, 2));
        break;
    }

    lua_rawget(L, -2); // push key value
    lua_replace(L, -2); // replace private instance table with value

    return 1;
}

int Graphics_RenderWindow_PropSet(lua_State *L) {
    auto _this = reinterpret_cast<RenderWindow*>(luaL_checkudata(L, 1, Graphics_RenderWindowMt));
    auto key = hash::fnv1<uint32_t>::hash(luaL_checkstring(L, 2));

    // push private instance table
    lua_pushlightuserdata(L, _this);
    lua_rawget(L, lua_upvalueindex(1));

    // check key
    switch (key) {
    case hash::fnv1<uint32_t>::hash(Graphics_RenderWindow_PropRenderer): {
        if (!lua_isnil(L, 3)) {
            auto value = Graphics_GraphicsRenderer_Internal_FromStack(L, 3);
            _this->SetRenderer(*value);
        }
        else {
            _this->SetRenderer(nullptr);
        }
        break;
    }
    default:
        luaL_error(L, "Can't access Graphics.RenderWindow.%s", lua_tostring(L, 2));
        break;
    }

    lua_pop(L, 1); // pop private instance table

    return 0;
}

void luaopen_Graphics_RenderWindow(lua_State *L) {
    static const luaL_Reg funcs[] = {
        { "New", Graphics_RenderWindow_new },
        { nullptr, nullptr }
    };

    static const luaL_Reg metaFuncs[] = {
        { "__index", Graphics_RenderWindow_PropGet },
        { "__newindex", Graphics_RenderWindow_PropSet },
        { "__gc", Graphics_RenderWindow_delete },
        { nullptr, nullptr }
    };

    luaL_checkversion(L);

    // shared upvalue for private class registry
    lua_newtable(L);

    auto ptr = lua_topointer(L, -1);

    luaL_newlibtable(L, funcs);
    lua_pushvalue(L, -2); // copy private class registry
    luaL_setfuncs(L, funcs, 1);

    luaL_newmetatable(L, Graphics_RenderWindowMt);
    lua_pushvalue(L, -3); // copy private class registry
    luaL_setfuncs(L, metaFuncs, 1);

    // pop metatable to leave only library table at the top of the stack
    lua_pop(L, 1);
    lua_replace(L, -2); // replace shared upvalue for private class registry with libtable that is on top of the stack and pop top
}