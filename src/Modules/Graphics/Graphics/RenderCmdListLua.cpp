#include "pch.h"
#include "RenderCmdListLua.h"
#include "Render/RenderCmdList.h"

#include <memory>
#include <LuaHelpers/Class.h>

static constexpr char *Graphics_RenderCmdListMt = "Graphics.RenderCmdList";

int Graphics_RenderCmdList_delete(lua_State *L) {
    auto _this = reinterpret_cast<RenderCmdList*>(luaL_checkudata(L, 1, Graphics_RenderCmdListMt));
    _this->~RenderCmdList();
    return 0;
}

int Graphics_RenderCmdList_new(lua_State *L) {
    auto mem = lua_newuserdata(L, sizeof RenderCmdList);

    try {
        new(mem) RenderCmdList();
    }
    catch (const std::exception &e) {
        std::string msg = "Failed to create RenderCmdList instance: ";
        msg += e.what();
        luaL_error(L, msg.c_str());
    }

    luaL_getmetatable(L, Graphics_RenderCmdListMt);
    lua_setmetatable(L, -2);

    return 1;
}

void luaopen_Graphics_RenderCmdList(lua_State *L) {
    static const luaL_Reg funcs[] = {
        { "New", Graphics_RenderCmdList_new },
        { nullptr, nullptr }
    };

    static const luaL_Reg metaFuncs[] = {
        { "__gc", Graphics_RenderCmdList_delete },
        { nullptr, nullptr }
    };

    LuaH::Class::Register(L, funcs, metaFuncs, Graphics_RenderCmdListMt);
}