#pragma once
#include "luaInc.h"

#ifdef __cplusplus
extern "C" {
#endif

    __declspec(dllexport) int luaopen_Graphics(lua_State *L);

#ifdef __cplusplus
}
#endif