#pragma once
#include "Render/CmdData/Color.h"
#include "Render/CmdData/Rect.h"

#include <LuaHelpers/luaInc.h>

namespace Lua {
    class DataReader {
    public:
        /*
        supported formats(checked in this order):
        - stack args : 4 numbers starting at idx - r, g, b, a
        - array { red : number, green : number, blue : number, alpha : number }
        - array { red : number, green : number, blue : number } implicit : alpha = 1.0
        - table (k:v) { r : number, g : number, b : number, a : number }
        - table (k:v) { r : number, g : number, b : number } implicit a = 1.0
        */
        static Color GetColor(lua_State *L, int idx, int *readStackItems = nullptr);

        /*
        supported formats(checked in this order):
        - stack args : 4 numbers starting at idx - left, top, right, bottom
        - array { left : number, top : number, right : number, bottom : number }
        - table (k:v) { left : number, top : number, right : number, bottom : number }
        */
        static Rect GetRect(lua_State *L, int idx, int *readStackItems = nullptr);
    };
}