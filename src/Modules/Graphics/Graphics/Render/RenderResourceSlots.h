#pragma once

#include <cstdint>

/*
Holds total number of resource slots for renderer.
When count changes renderer must preserve old resources so you can use old slots.
To delete all slots just set count to 0.
*/
struct RenderResourceSlots {
    size_t rect;
    size_t colorBrush;
};