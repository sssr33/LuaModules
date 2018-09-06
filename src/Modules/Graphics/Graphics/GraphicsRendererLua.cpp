#include "pch.h"
#include "GraphicsRendererLua.h"
#include "GraphicsRendererLuaInternal.h"
#include "GraphicsRenderer.h"

#include <memory>
#include <LuaHelpers/Class.h>
#include <LuaHelpers/Error.h>

namespace Lua {
    struct GraphicsRendererLua {
        static constexpr char *NameMt = "Graphics.GraphicsRenderer";

        struct Prop {
        };

        static int Create(lua_State *L) {
            try {
                auto renderer = std::make_shared<GraphicsRenderer>();
                return Internal::GraphicsRendererLua::Create(L, std::move(renderer));
            }
            catch (const std::exception &e) {
                LuaH::Error::FailedToCreate(L, NameMt, e);
            }

            return 0;
        }

        static int Destroy(lua_State *L) {
            auto _this = Internal::GraphicsRendererLua::GetFromStack(L, 1);
            _this->~shared_ptr();
            return 0;
        }

        static int PropGet(lua_State *L) {
            auto top = lua_gettop(L);
            bool t = lua_istable(L, -2);
            bool s = lua_isstring(L, -1);

            auto tv = lua_topointer(L, -2);
            auto sv = lua_tostring(L, -1);

            luaL_getmetatable(L, NameMt);
            auto tv2 = lua_topointer(L, -1);

            bool calledWithMt = tv == tv2;

            return 0;
        }

        static int PropSet(lua_State *L) {
            return 0;
        }
    };

    void LuaOpenGraphicsRenderer(lua_State *L) {
        static const luaL_Reg funcs[] = {
            { "New", GraphicsRendererLua::Create },
            { nullptr, nullptr }
        };

        static const luaL_Reg metaFuncs[] = {
            { "__gc", GraphicsRendererLua::Destroy },
            { nullptr, nullptr }
        };

        LuaH::Class::Register(L, funcs, metaFuncs, GraphicsRendererLua::NameMt);
    }

    namespace Internal {
        int GraphicsRendererLua::Create(lua_State *L, std::shared_ptr<GraphicsRenderer> renderer) {
            auto mem = lua_newuserdata(L, sizeof std::shared_ptr<GraphicsRenderer>);

            try {
                new(mem) std::shared_ptr<GraphicsRenderer>(std::move(renderer));
            }
            catch (const std::exception &e) {
                LuaH::Error::FailedToCreate(L, Lua::GraphicsRendererLua::NameMt, e);
            }

            luaL_getmetatable(L, Lua::GraphicsRendererLua::NameMt);
            lua_setmetatable(L, -2);

            return 1;
        }

        std::shared_ptr<GraphicsRenderer> *GraphicsRendererLua::GetFromStack(lua_State *L, int idx) {
            auto _this = (std::shared_ptr<GraphicsRenderer>*)luaL_checkudata(L, idx, Lua::GraphicsRendererLua::NameMt);
            return _this;
        }
    }
}