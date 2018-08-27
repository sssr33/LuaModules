#include "pch.h"
#include "GraphicsRenderer.h"

GraphicsRenderer::GraphicsRenderer() {
}

DxDevice *GraphicsRenderer::GetDxDevice() {
    return &this->dxDev;
}

void GraphicsRenderer::SetResourceSlots(const RenderResourceSlots &count) {
    this->brushes.resize(count.colorBrush);
    this->rects.resize(count.rect);
}

void GraphicsRenderer::Render(RenderCmdList &renderCmd) {
    auto ctxLk = this->dxDev.LockCtxScoped();
    auto d2d = this->dxDev.D2D();

    Renderer renderer;
    renderer.base = this;

    HRESULT hr = S_OK;
    d2d->BeginDraw();
    renderCmd.Accept(renderer);
    hr = d2d->EndDraw();
}

void GraphicsRenderer::Renderer::visit(ClearScreenCmd &cmd) {
    auto color = D2D1::ColorF(cmd.color.r, cmd.color.g, cmd.color.b, cmd.color.a);
    auto d2d = base->dxDev.D2D();

    d2d->Clear(color);
}

void GraphicsRenderer::Renderer::visit(RenderRectCmd &cmd) {
    auto &rect = base->rects[cmd.rectId];
    auto &brush = base->brushes[cmd.brushId];
    auto d2d = base->dxDev.D2D();

    if (cmd.fill) {
        d2d->FillRectangle(rect, brush.Get());
    }
    else {
        d2d->DrawRectangle(rect, brush.Get());
    }
}

void GraphicsRenderer::Renderer::visit(SetBrushColorCmd &cmd) {
    auto color = D2D1::ColorF(cmd.color.r, cmd.color.g, cmd.color.b, cmd.color.a);
    auto &brush = base->brushes[cmd.brushId];

    if (brush) {
        brush->SetColor(color);
    }
    else {
        HRESULT hr = S_OK;
        auto d2d = base->dxDev.GetD2DCtxMt();

        hr = d2d->CreateSolidColorBrush(color, brush.GetAddressOf());
        if (FAILED(hr)) {
            throw std::exception();
        }
    }
}

void GraphicsRenderer::Renderer::visit(SetRectCmd &cmd) {
    auto rect = D2D1::RectF(cmd.rect.left, cmd.rect.top, cmd.rect.right, cmd.rect.bottom);
    base->rects[cmd.rectId] = rect;
}