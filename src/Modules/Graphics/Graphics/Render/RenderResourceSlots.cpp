#include "pch.h"
#include "RenderResourceSlots.h"

RenderResourceSlots::RenderResourceSlots()
    : rect(0), colorBrush(0)
{}

bool RenderResourceSlots::Empty() const {
    bool empty = this->rect == 0 && this->colorBrush == 0;
    return empty;
}