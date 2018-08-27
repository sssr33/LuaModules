#include "pch.h"
//#include "RenderCmdList.h"
//
//RenderCmdList::RenderCmdList()
//    : cmdClearScreen(CmdMemInfoType::ClearScreen, &this->cmdRenderList)
//    , cmdRenderRect(CmdMemInfoType::RenderRect, &this->cmdRenderList)
//{
//
//}
//
//void RenderCmdList::Clear() {
//    this->cmdRenderList.clear();
//    this->cmdClearScreen.clear();
//    this->cmdRenderRect.clear();
//}
//
//void RenderCmdList::Add(ClearScreenCmd cmd) {
//    this->cmdClearScreen.push_back(std::move(cmd));
//}
//
//void RenderCmdList::Add(RenderRectCmd cmd) {
//    this->cmdRenderRect.push_back(std::move(cmd));
//}
//
//void RenderCmdList::Test() {
//    std::array<CmdMemInfo, (size_t)CmdMemInfoType::Count> cmdMem;
//
//    cmdMem[this->cmdClearScreen.GetCmdMemInfoTypeIdx()] = this->cmdClearScreen.GetMemInfo();
//    cmdMem[this->cmdRenderRect.GetCmdMemInfoTypeIdx()] = this->cmdRenderRect.GetMemInfo();
//
//    IRenderCmd *a = &this->cmdClearScreen[1];
//    IRenderCmd *b = &this->cmdRenderRect[1];
//
//    auto a2 = cmdMem[this->cmdClearScreen.GetCmdMemInfoTypeIdx()][1];
//    auto b2 = cmdMem[this->cmdRenderRect.GetCmdMemInfoTypeIdx()][1];
//
//    bool aOk = a2 == a;
//    bool bOk = b2 == b;
//
//    int stop = 23;
//}