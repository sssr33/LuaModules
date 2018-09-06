#include "pch.h"
#include "RenderResourceSlotsLua.h"
#include "Render/RenderResourceSlots.h"

#include <memory>
#include <string>
#include <libhelpers/fnv1.h>
#include <LuaHelpers/Class.h>
#include <LuaHelpers/Error.h>

namespace Lua {
    struct RenderResourceSlotsLua {
        typedef RenderResourceSlots CType;
        static constexpr char *NameMt = "Graphics.RenderResourceSlots";

        struct Prop {
            static constexpr char *Rect = "Rect";
            static constexpr char *ColorBrush = "ColorBrush";
        };

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

        static int PropGet(lua_State *L) {
            auto _this = reinterpret_cast<CType*>(luaL_checkudata(L, 1, NameMt));
            auto key = hash::fnv1<uint32_t>::hash(luaL_checkstring(L, 2));

            switch (key) {
            case hash::fnv1<uint32_t>::hash(Prop::Rect): {
                lua_pushinteger(L, _this->rect);
                break;
            }
            case hash::fnv1<uint32_t>::hash(Prop::ColorBrush): {
                lua_pushinteger(L, _this->colorBrush);
                break;
            }
            default:
                lua_pushnil(L);
                break;
            }

            return 1;
        }

        static int PropSet(lua_State *L) {
            auto _this = reinterpret_cast<CType*>(luaL_checkudata(L, 1, NameMt));
            auto key = hash::fnv1<uint32_t>::hash(luaL_checkstring(L, 2));

            switch (key) {
            case hash::fnv1<uint32_t>::hash(Prop::Rect): {
                auto val = luaL_checkinteger(L, 3);
                _this->rect = (size_t)val;
                break;
            }
            case hash::fnv1<uint32_t>::hash(Prop::ColorBrush): {
                auto val = luaL_checkinteger(L, 3);
                _this->colorBrush = (size_t)val;
                break;
            }
            default:
                LuaH::Error::FailedPropertySetter(L, NameMt);
                break;
            }

            return 0;
        }
    };

    void LuaOpenRenderResourceSlots(lua_State *L) {
        static const luaL_Reg funcs[] = {
            { "New", RenderResourceSlotsLua::Create },
            { nullptr, nullptr }
        };

        static const luaL_Reg metaFuncs[] = {
            { "__index", RenderResourceSlotsLua::PropGet },
            { "__newindex", RenderResourceSlotsLua::PropSet },
            { "__gc", RenderResourceSlotsLua::Destroy },
            { nullptr, nullptr }
        };

        LuaH::Class::Register(L, funcs, metaFuncs, RenderResourceSlotsLua::NameMt);
    }
}