#include "pch.h"
#include "RenderCmdListLua.h"
#include "DataReaderLua.h"
#include "Render/RenderCmdList.h"

#include <memory>
#include <LuaHelpers/Class.h>
#include <LuaHelpers/Error.h>
#include <LuaHelpers/DataReader.h>

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

        static int ClearScreen(lua_State *L) {
            auto _this = reinterpret_cast<CType*>(luaL_checkudata(L, 1, NameMt));
            ClearScreenCmd cmd;

            cmd.color = DataReader::GetColor(L, 2);

            _this->Add(std::move(cmd));
            return 0;
        }

        // params: num rectId, num brushId, bool fill
        static int RenderRect(lua_State *L) {
            auto _this = reinterpret_cast<CType*>(luaL_checkudata(L, 1, NameMt));
            RenderRectCmd cmd;

            cmd.rectId = LuaH::DataReader::GetStack<uint32_t>(L, 2, "rectId");
            cmd.brushId = LuaH::DataReader::GetStack<uint32_t>(L, 3, "brushId");
            cmd.fill = LuaH::DataReader::GetStack<bool>(L, 4, "fill");

            _this->Add(std::move(cmd));
            return 0;
        }

        static int SetBrushColor(lua_State *L) {
            auto _this = reinterpret_cast<CType*>(luaL_checkudata(L, 1, NameMt));
            SetBrushColorCmd cmd;



            _this->Add(std::move(cmd));
            return 0;
        }

        static int SetRect(lua_State *L) {
            auto _this = reinterpret_cast<CType*>(luaL_checkudata(L, 1, NameMt));
            SetRectCmd cmd;


            _this->Add(std::move(cmd));
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
            { "ClearScreen", RenderCmdListLua::ClearScreen },
            { "RenderRect", RenderCmdListLua::RenderRect },
            { "SetBrushColor", RenderCmdListLua::SetBrushColor },
            { "SetRect", RenderCmdListLua::SetRect },
            { nullptr, nullptr }
        };

        LuaH::Class::Register(L, funcs, metaFuncs, RenderCmdListLua::NameMt, true);
    }
}