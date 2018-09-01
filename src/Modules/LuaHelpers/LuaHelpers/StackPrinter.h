#pragma once
#include "luaInc.h"

#include <string>
#include <array>

namespace LuaH {
    class StackPrinter {
    public:
        static std::string Print(lua_State *L, uint32_t depth = 4);

    private:
        struct PrintState {
            std::array<char, 32> buf;
            std::string res;

            void PrintType(lua_State *L, int index, uint32_t depth);

        private:
            void PrintTypeHelper(lua_State *L, int index, uint32_t depth, const std::string &indent, const std::string &indentInc);
            void PrintPtr(lua_State *L, int index, const char *fmt);
            void PrintUserData(lua_State *L, int index);
            void PrintTable(lua_State *L, int index, uint32_t depth, const std::string &indent, const std::string &indentInc);
        };
    };
}