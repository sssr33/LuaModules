#pragma once
#include "GraphicsRenderer.h"

#include <memory>
#include <LuaHelpers/luaInc.h>

int Graphics_GraphicsRenderer_Internal_new(lua_State *L, std::shared_ptr<GraphicsRenderer> renderer);
std::shared_ptr<GraphicsRenderer> *Graphics_GraphicsRenderer_Internal_FromStack(lua_State *L, int idx);