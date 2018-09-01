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
        index = lua_absindex(L, index);

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
            this->PrintUserData(L, index);
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

    void StackPrinter::PrintState::PrintUserData(lua_State *L, int index) {
        assert(index > 0);

        lua_getmetatable(L, index);

        std::string name;

        if (lua_istable(L, -1)) {
            // got metatable
            // search registry for userdata metatable

            int mtIdx = lua_gettop(L);

            lua_pushnil(L);  // first key

            while (lua_next(L, LUA_REGISTRYINDEX) != 0) {
                const int keyIdx = -2;
                const int valIdx = -1;

                bool found = lua_rawequal(L, -1, mtIdx) != 0;

                // removes 'value'; keeps 'key' for next iteration
                lua_pop(L, 1);

                if (found) {
                    // use another PrintState to print any key type
                    PrintState tmpState;

                    tmpState.PrintType(L, -1, 0);
                    name = std::move(tmpState.res);

                    lua_pop(L, 1); // pop key
                    break;
                }
            }
        }

        lua_pop(L, 1); // pop metatable

        if (!name.empty()) {
            this->PrintPtr(L, index, "%#016x(USERDATA:");
            this->res += name;
            this->res += ")";
        }
        else {
            this->PrintPtr(L, index, "%#016x(USERDATA)");
        }
    }

    void StackPrinter::PrintState::PrintTable(lua_State *L, int index, uint32_t depth, const std::string &indent, const std::string &indentInc) {
        assert(index > 0);

        bool first = true;

        this->PrintPtr(L, index, "%#016x(TABLE)");

        lua_pushnil(L);  // first key

        while (lua_next(L, index) != 0) {
            // PrintTypeHelper expects positive indexes
            // Get absolute indexes from current top
            const int keyIdx = lua_absindex(L, -2);
            const int valIdx = lua_absindex(L, -1);

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

            // removes 'value'; keeps 'key' for next iteration
            lua_pop(L, 1);
        }
    }
}