#include "pch.h"
#include "GraphicsRendererLua.h"
#include "GraphicsRendererLuaInternal.h"
#include "RenderResourceSlotsLuaInternal.h"
#include "RenderCmdListLuaInternal.h"
#include "GraphicsRenderer.h"

#include <memory>
#include <LuaHelpers/Class.h>
#include <LuaHelpers/Error.h>

namespace Lua {
    struct GraphicsRendererLua {
        static constexpr char *NameMt = "Graphics.GraphicsRenderer";

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

        /*static int SetResourceSlots(lua_State *L) {
            auto _this = Internal::GraphicsRendererLua::GetFromStack(L, 1);
            auto resSlots = Internal::RenderResourceSlotsLua::GetFromStack(L, 2);

            (*_this)->SetResourceSlots(*resSlots);

            return 0;
        }

        static int SetRenderCmdList(lua_State *L) {
            auto _this = Internal::GraphicsRendererLua::GetFromStack(L, 1);
            auto resSlots = Internal::RenderCmdListLua::GetFromStack(L, 2);

            (*_this)->SetRenderCmdList(*resSlots);

            return 0;
        }*/
    };

    void LuaOpenGraphicsRenderer(lua_State *L) {
        static const luaL_Reg funcs[] = {
            { "New", GraphicsRendererLua::Create },
            { nullptr, nullptr }
        };

        static const luaL_Reg metaFuncs[] = {
            { "__gc", GraphicsRendererLua::Destroy },
            /*{ "SetResourceSlots", GraphicsRendererLua::SetResourceSlots },
            { "SetRenderCmdList", GraphicsRendererLua::SetRenderCmdList },*/
            { nullptr, nullptr }
        };

        LuaH::Class::Register(L, funcs, metaFuncs, GraphicsRendererLua::NameMt, true);
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
            auto _this = static_cast<std::shared_ptr<GraphicsRenderer>*>(luaL_checkudata(L, idx, Lua::GraphicsRendererLua::NameMt));
            return _this;
        }
    }
}