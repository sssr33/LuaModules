#pragma once
#include "../CmdData/Rect.h"

#include <cstdint>

struct SetRectCmd {
    Rect rect;
    uint32_t rectId;
};