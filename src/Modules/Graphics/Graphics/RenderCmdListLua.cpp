#include "pch.h"
#include "RenderCmdListLua.h"
#include "Render/RenderCmdList.h"

#include <memory>
#include <LuaHelpers/Class.h>
#include <LuaHelpers/Error.h>

namespace Lua {
    struct RenderCmdListLua {
        typedef RenderCmdList CType;
        static constexpr char *NameMt = "Graphics.RenderCmdList";

        static int Create(lua_State *L) {
            auto mem = lua_newuserdata(L, sizeof CType);

            try {
                new(mem) CType();
            }
            catch (const std::exception &e) {
                LuaH::Error::FailedToCreate(L, NameMt, e);
            }

            luaL_getmetatable(L, NameMt);
            lua_setmetatable(L, -2);

            return 1;
        }

        static int Destroy(lua_State *L) {
            auto _this = reinterpret_cast<CType*>(luaL_checkudata(L, 1, NameMt));
            _this->~CType();
            return 0;
        }
    };

    void LuaOpenRenderCmdList(lua_State *L) {
        static const luaL_Reg funcs[] = {
            { "New", RenderCmdListLua::Create },
            { nullptr, nullptr }
        };

        static const luaL_Reg metaFuncs[] = {
            { "__gc", RenderCmdListLua::Destroy },
            { nullptr, nullptr }
        };

        LuaH::Class::Register(L, funcs, metaFuncs, RenderCmdListLua::NameMt);
    }
}