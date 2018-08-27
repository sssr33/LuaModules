#pragma once

#include <cstdint>

class RenderRectCmd {
public:
    uint32_t rectId;
    uint32_t brushId;
    bool fill;
};