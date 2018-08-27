#pragma once
#include "luaInc.h"
#include "GraphicsRenderer.h"

#include <memory>

int Graphics_GraphicsRenderer_Internal_new(lua_State *L, std::shared_ptr<GraphicsRenderer> renderer);
std::shared_ptr<GraphicsRenderer> *Graphics_GraphicsRenderer_Internal_FromStack(lua_State *L, int idx);