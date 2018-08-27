#pragma once
//#include "ClearScreenCmd.h"
//#include "RenderRectCmd.h"
//
//#include <vector>
//#include <array>
//
//class RenderCmdList {
//    enum class CmdMemInfoType : size_t {
//        ClearScreen,
//        RenderRect,
//
//        Count
//    };
//public:
//    RenderCmdList();
//
//    void Clear();
//
//    void Add(ClearScreenCmd cmd);
//    void Add(RenderRectCmd cmd);
//
//    void Test();
//
//private:
//    struct RenderCmdIdx {
//        CmdMemInfoType type;
//        size_t idx;
//    };
//
//    struct CmdMemInfo {
//        size_t stride;
//        IRenderCmd *memory;
//
//        IRenderCmd *operator[](const size_t i) {
//            uint8_t *memTmp = (uint8_t *)this->memory;
//            memTmp += i * this->stride;
//            return (IRenderCmd *)memTmp;
//        }
//    };
//
//    template<class T>
//    class CmdMem {
//    public:
//        CmdMem()
//            : type(CmdMemInfoType::Count)
//            , cmdRenderList(nullptr)
//        {}
//
//        CmdMem(
//            CmdMemInfoType type,
//            std::vector<RenderCmdIdx> *cmdRenderList)
//            : type(type)
//            , cmdRenderList(cmdRenderList)
//        {}
//
//        CmdMemInfo GetMemInfo() {
//            CmdMemInfo memInfo;
//
//            memInfo.stride = sizeof T;
//            memInfo.memory = this->cmd.data();
//
//            return memInfo;
//        }
//
//        T &operator[](const size_t i) {
//            return this->cmd[i];
//        }
//
//        size_t GetCmdMemInfoTypeIdx() const {
//            return (size_t)this->type;
//        }
//
//        void clear() {
//            this->cmd.clear();
//        }
//
//        void push_back(const T &v) {
//            this->AddRenderCmdIdx();
//            this->cmd.push_back(v);
//        }
//
//        void push_back(T &&v) {
//            this->AddRenderCmdIdx();
//            this->cmd.push_back(std::move(v));
//        }
//
//    private:
//        CmdMemInfoType type;
//        std::vector<RenderCmdIdx> *cmdRenderList;
//        std::vector<T> cmd;
//
//        void AddRenderCmdIdx() {
//            RenderCmdIdx idx;
//
//            idx.type = this->type;
//            idx.idx = this->cmd.size();
//
//            cmdRenderList->push_back(std::move(idx));
//        }
//    };
//
//    std::vector<RenderCmdIdx> cmdRenderList;
//
//    CmdMem<ClearScreenCmd> cmdClearScreen;
//    CmdMem<RenderRectCmd> cmdRenderRect;
//};