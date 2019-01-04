#pragma once
#include "Render/RenderCmdList.h"
#include "Render/RenderResourceSlots.h"

#include <libhelpers/Dx/DxDevice.h>

class GraphicsRenderer {
public:
    GraphicsRenderer();

    DxDevice *GetDxDevice();

    /*
    Use this method to allocate empty resource id's or clean resources.
    After resource has been set use <Render> to assign actual data to slots.
    */
    void SetResourceSlots(const RenderResourceSlots &count);
    void SetResourceSlotsNoCtxLock(const RenderResourceSlots &count);

    ///*
    //Render render commands to be issued by Render method.
    //*/
    //void SetRenderCmdList(RenderCmdList list);
    //void SetRenderCmdListNoCtxLock(RenderCmdList list);

    /*
    Performs render commands.
    */
    void Render(RenderCmdList &list);
    void RenderNoCtxLock(RenderCmdList &list);

private:
    DxDevice dxDev;

    std::vector<D2D1_RECT_F> rects;
    std::vector<Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>> brushes;
    //RenderCmdList renderCmd;

    struct Renderer {
        GraphicsRenderer *base;

        void visit(ClearScreenCmd &cmd);
        void visit(RenderRectCmd &cmd);
        void visit(SetBrushColorCmd &cmd);
        void visit(SetRectCmd &cmd);
    };
};