#pragma once
#include "../CmdData/Color.h"

#include <cstdint>

class SetBrushColorCmd {
public:
    Color color;
    uint32_t brushId;
};