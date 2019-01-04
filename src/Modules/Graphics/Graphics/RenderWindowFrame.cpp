#include "pch.h"
#include "RenderWindowFrame.h"

RenderWindowFrame::RenderWindowFrame()
{}

RenderResourceSlots RenderWindowFrame::DetachSlots() {
    auto tmp = std::move(this->resSlots);
    return tmp;
}

const RenderResourceSlots &RenderWindowFrame::GetSlots() const {
    return this->resSlots;
}

void RenderWindowFrame::SetSlots(RenderResourceSlots slots) {
    this->resSlots = std::move(slots);
}