#include "InstanceTable.h"

namespace LuaH {
    void InstanceTable::PropGet(lua_State *L, int userDataIdx, int keyIdx) {
        // push private instance table
        lua_getuservalue(L, userDataIdx);

        // do not check key just use what we've got
        // if private instance table doesn't have it nil will be on the stack
        lua_pushvalue(L, keyIdx); // push key
        lua_rawget(L, -2); // push key value
        lua_replace(L, -2); // replace private instance table with value
    }

    void InstanceTable::PropSet(lua_State *L, int userDataIdx, int keyIdx, int valueIdx) {
        // push private instance table
        lua_getuservalue(L, userDataIdx);
        lua_pushvalue(L, keyIdx); // push key
        lua_pushvalue(L, valueIdx); // push value
        lua_rawset(L, -3);
        lua_pop(L, 1); // pop private instance table
    }
}