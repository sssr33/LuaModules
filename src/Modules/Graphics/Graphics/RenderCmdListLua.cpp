#include "pch.h"
#include "RenderCmdListLua.h"
#include "RenderCmdListLuaInternal.h"
#include "DataReaderLua.h"
#include "Render/RenderCmdList.h"

#include <memory>
#include <LuaHelpers/Class.h>
#include <LuaHelpers/Error.h>
#include <LuaHelpers/DataReader.h>

namespace Lua {
    struct RenderCmdListLua {
        typedef Internal::RenderCmdListLua::CType CType;
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
            auto _this = Internal::RenderCmdListLua::GetFromStack(L, 1);
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

        // params: uint32 rectId, uint32 brushId, bool fill
        static int RenderRect(lua_State *L) {
            auto _this = Internal::RenderCmdListLua::GetFromStack(L, 1);
            RenderRectCmd cmd;

            cmd.rectId = LuaH::DataReader::GetStack<uint32_t>(L, 2, "rectId");
            cmd.brushId = LuaH::DataReader::GetStack<uint32_t>(L, 3, "brushId");
            cmd.fill = LuaH::DataReader::GetStack<bool>(L, 4, "fill");

            _this->Add(std::move(cmd));
            return 0;
        }

        // params: Color color, uint32 brushId
        static int SetBrushColor(lua_State *L) {
            auto _this = Internal::RenderCmdListLua::GetFromStack(L, 1);
            SetBrushColorCmd cmd;
            int stackIdx = 2;
            int readStackItems = 0;

            cmd.color = DataReader::GetColor(L, stackIdx, &readStackItems);
            stackIdx += readStackItems;

            if (readStackItems == 4) {
                bool defaultUsed = false;

                // (uint32_t)0 - without this cast overload with nullptr will be used
                cmd.brushId = LuaH::DataReader::GetStack<uint32_t>(L, stackIdx, "brushId", (uint32_t)0, &defaultUsed);

                if (defaultUsed) {
                    // take into account such arguments: r, g, b, brushId
                    cmd.brushId = LuaH::DataReader::GetStack<uint32_t>(L, stackIdx - 1, "brushId");
                    cmd.color.a = 1.f;
                }
                else {
                    // r, g, b, a, brushId
                    stackIdx++; // succeeded to read
                }
            }
            else {
                // {r, g, b, (a)}, brushId
                cmd.brushId = LuaH::DataReader::GetStack<uint32_t>(L, stackIdx, "brushId");
            }

            _this->Add(std::move(cmd));
            return 0;
        }

        // params: Rect rect, uint32 rectId
        static int SetRect(lua_State *L) {
            auto _this = Internal::RenderCmdListLua::GetFromStack(L, 1);
            SetRectCmd cmd;
            int stackIdx = 2;
            int readStackItems = 0;

            cmd.rect = DataReader::GetRect(L, stackIdx, &readStackItems);
            stackIdx += readStackItems;
            cmd.rectId = LuaH::DataReader::GetStack<uint32_t>(L, stackIdx, "rectId");

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

    namespace Internal {
        RenderCmdListLua::CType *RenderCmdListLua::GetFromStack(lua_State *L, int idx) {
            auto _this = static_cast<CType*>(luaL_checkudata(L, idx, Lua::RenderCmdListLua::NameMt));
            return _this;
        }
    }
}