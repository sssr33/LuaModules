#pragma once
#include "Render/RenderCmdList.h"
#include "Render/RenderResourceSlots.h"

class RenderWindowFrame {
public:
    RenderCmdList cmdList;

    RenderWindowFrame();

    RenderResourceSlots DetachSlots();
    const RenderResourceSlots &GetSlots() const;
    void SetSlots(RenderResourceSlots slots);

private:
    RenderResourceSlots resSlots;
};