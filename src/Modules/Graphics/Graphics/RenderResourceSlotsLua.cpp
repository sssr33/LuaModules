#include "pch.h"
#include "RenderResourceSlotsLua.h"
#include "Render/RenderResourceSlots.h"

#include <memory>
#include <string>
#include <LuaHelpers/Class.h>

static constexpr char *Graphics_RenderResourceSlotsMt = "Graphics.RenderResourceSlots";

int Graphics_RenderResourceSlots_delete(lua_State *L) {
    auto _this = reinterpret_cast<RenderResourceSlots*>(luaL_checkudata(L, 1, Graphics_RenderResourceSlotsMt));
    _this->~RenderResourceSlots();
    return 0;
}

int Graphics_RenderResourceSlots_new(lua_State *L) {
    auto mem = lua_newuserdata(L, sizeof RenderResourceSlots);

    try {
        new(mem) RenderResourceSlots();
    }
    catch (const std::exception &e) {
        std::string msg = "Failed to create RenderResourceSlots instance: ";
        msg += e.what();
        luaL_error(L, msg.c_str());
    }

    luaL_getmetatable(L, Graphics_RenderResourceSlotsMt);
    lua_setmetatable(L, -2);

    return 1;
}

void luaopen_Graphics_RenderResourceSlots(lua_State *L) {
    static const luaL_Reg funcs[] = {
        { "New", Graphics_RenderResourceSlots_new },
        { nullptr, nullptr }
    };

    static const luaL_Reg metaFuncs[] = {
        { "__gc", Graphics_RenderResourceSlots_delete },
        { nullptr, nullptr }
    };

    LuaH::Class::Register(L, funcs, metaFuncs, Graphics_RenderResourceSlotsMt);
}