#pragma once
#include "luaInc.h"

namespace LuaH {
    /*
    Wrapper for class instance table.
    This table used to implement properties in C/C++ types with help of __index and __newindex metamethods.
    */
    class InstanceTable {
    public:
        // Creates table(stack -1), calls <initFn> and sets it to userData at <userDataIdx>
        // <initFn> must NOT leave anything on the stack!
        // <initFn> used to initialize initial values of properties. This is intended for object properties so the object will not need to be copied on each Get.
        // If lua class needs only value properties(numbers, strings and other primitive types) then it doesn't need to init instance table.
        // Thi table will be destroyed automatically by lua runtime with userdata.
        template<class Fn> static void Init(lua_State *L, Fn initFn, int userDataIdx = -2) {
            // create private instance table
            lua_newtable(L);

            // fill private instance table
            initFn();

            // set private instance table as uservalue of userdata
            lua_setuservalue(L, userDataIdx);
        }

        // Gets value from instance table and leaves it on the stack
        static void PropGet(lua_State *L, int userDataIdx = 1, int keyIdx = 2);

        // Updates(sets) value in instance table and
        static void PropSet(lua_State *L, int userDataIdx = 1, int keyIdx = 2, int valueIdx = 3);
    };
}