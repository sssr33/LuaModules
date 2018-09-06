#include "pch.h"
#include "RenderWindowLua.h"
#include "RenderWindow.h"
#include "GraphicsRendererLuaInternal.h"

#include <libhelpers/UtfConvert.h>
#include <libhelpers/fnv1.h>
#include <LuaHelpers/InstanceTable.h>
#include <LuaHelpers/Class.h>
#include <LuaHelpers/Error.h>

namespace Lua {
    struct RenderWindowLua {
        typedef RenderWindow CType;
        static constexpr char *NameMt = "Graphics.RenderWindow";

        struct Prop {
            static constexpr char *Renderer = "Renderer";
        };

        static int Create(lua_State *L) {
            std::wstring title = L"RenderWindow";

            if (lua_isstring(L, 1)) {
                title = ConvertToUtf16(lua_tostring(L, 1));
            }

            auto mem = lua_newuserdata(L, sizeof CType);

            try {
                new(mem) CType(std::move(title));
            }
            catch (const std::exception &e) {
                LuaH::Error::FailedToCreate(L, NameMt, e);
            }

            luaL_getmetatable(L, NameMt);
            lua_setmetatable(L, -2);

            LuaH::InstanceTable::Init(L, [&] {
                auto obj = reinterpret_cast<CType*>(mem);

                auto renderer = obj->GetRenderer();
                Internal::GraphicsRendererLua::Create(L, std::move(renderer));
                lua_setfield(L, -2, Prop::Renderer);
            });

            return 1;
        }

        static int Destroy(lua_State *L) {
            auto _this = reinterpret_cast<CType*>(luaL_checkudata(L, 1, NameMt));
            _this->~CType();
            return 0;
        }

        static int PropGet(lua_State *L) {
            auto _this = reinterpret_cast<CType*>(luaL_checkudata(L, 1, NameMt));

            LuaH::InstanceTable::PropGet(L);

            return 1;
        }

        static int PropSet(lua_State *L) {
            auto _this = reinterpret_cast<CType*>(luaL_checkudata(L, 1, NameMt));
            auto key = hash::fnv1<uint32_t>::hash(luaL_checkstring(L, 2));

            // check key
            switch (key) {
            case hash::fnv1<uint32_t>::hash(Prop::Renderer): {
                if (!lua_isnil(L, 3)) {
                    auto value = Internal::GraphicsRendererLua::GetFromStack(L, 3);
                    _this->SetRenderer(*value);
                }
                else {
                    _this->SetRenderer(nullptr);
                }
                break;
            }
            default:
                LuaH::Error::FailedPropertySetter(L, NameMt);
                break;
            }

            LuaH::InstanceTable::PropSet(L);

            return 0;
        }
    };

    void LuaOpenRenderWindow(lua_State *L) {
        static const luaL_Reg funcs[] = {
            { "New", RenderWindowLua::Create },
            { nullptr, nullptr }
        };

        static const luaL_Reg metaFuncs[] = {
            { "__index", RenderWindowLua::PropGet },
            { "__newindex", RenderWindowLua::PropSet },
            { "__gc", RenderWindowLua::Destroy },
            { nullptr, nullptr }
        };

        LuaH::Class::Register(L, funcs, metaFuncs, RenderWindowLua::NameMt);
    }
}