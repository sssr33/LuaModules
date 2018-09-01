#include "StackPrinter.h"

#include <assert.h>

namespace LuaH {
    std::string StackPrinter::Print(lua_State *L, uint32_t depth) {
        PrintState printState;
        int top = lua_gettop(L);

        for (int i = 0; i < top; i++) {
            int invIdx = -1 - i;

            printState.res += "[" + std::to_string(invIdx) + "] = ";
            printState.PrintType(L, invIdx, depth);
            printState.res += '\n';
        }

        return std::move(printState.res);
    }

    void StackPrinter::PrintState::PrintType(lua_State *L, int index, uint32_t depth) {
        if (index < 0) {
            // make index positive to have constant position
            index = lua_gettop(L) + 1 + index;
        }

        this->PrintTypeHelper(L, index, depth, "", "    ");
    }

    void StackPrinter::PrintState::PrintTypeHelper(lua_State *L, int index, uint32_t depth, const std::string &indent, const std::string &indentInc) {
        assert(index > 0);

        int type = lua_type(L, index);

        switch (type) {
        case LUA_TNIL:
            this->res += "nil";
            break;
        case LUA_TBOOLEAN:
            this->res += lua_toboolean(L, index) ? "true" : "false";
            break;
        case LUA_TLIGHTUSERDATA:
            this->PrintPtr(L, index, "%#016x(LIGHTUSERDATA)");
            break;
        case LUA_TNUMBER: {
            auto num = lua_tonumber(L, index);
            this->res += std::to_string(num);
            break;
        }
        case LUA_TSTRING:
            this->res += lua_tostring(L, index);
            break;
        case LUA_TTABLE:
            if (depth > 0) {
                this->PrintTable(L, index, depth - 1, indent + indentInc, indentInc);
            }
            else {
                this->PrintPtr(L, index, "%#016x(TABLE)");
            }
            break;
        case LUA_TFUNCTION:
            this->PrintPtr(L, index, "%#016x(FUNCTION)");
            break;
        case LUA_TUSERDATA:
            this->PrintPtr(L, index, "%#016x(USERDATA)");
            break;
        case LUA_TTHREAD:
            this->PrintPtr(L, index, "%#016x(THREAD)");
            break;
        default:
            this->res += "unknown";
            break;
        }
    }

    void StackPrinter::PrintState::PrintPtr(lua_State *L, int index, const char *fmt) {
        assert(index > 0);

        snprintf(this->buf.data(), this->buf.size(), fmt, lua_topointer(L, index));
        this->res += this->buf.data();
    }

    void StackPrinter::PrintState::PrintTable(lua_State *L, int index, uint32_t depth, const std::string &indent, const std::string &indentInc) {
        assert(index > 0);

        bool first = true;

        this->PrintPtr(L, index, "%#016x(TABLE)");

        lua_pushnil(L);  // first key

        while (lua_next(L, index) != 0) {
            const int keyIdx = index + 1; // or -2;
            const int valIdx = index + 2; // or -1;

            if (first) {
                // table has some entries, add ':\n' in front of '(TABLE)'
                this->res += ":\n";
            }
            else {
                // add next line for previous entry
                this->res += '\n';
            }

            this->res += indent + "[";
            this->PrintTypeHelper(L, keyIdx, 0, indent, indentInc);
            this->res += "] = ";
            this->PrintTypeHelper(L, valIdx, depth, indent, indentInc);
            first = false;

            /* removes 'value'; keeps 'key' for next iteration */
            lua_pop(L, 1);
        }
    }
}