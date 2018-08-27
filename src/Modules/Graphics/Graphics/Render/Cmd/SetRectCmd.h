#pragma once
#include "../CmdData/Rect.h"

#include <cstdint>

struct SetRectCmd {
    uint32_t rectId;
    Rect rect;
};