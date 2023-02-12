#pragma once
#include "Interface.hpp"

// 运行模块
namespace My {
    Interface IRuntimeModule{
        public:
            virtual ~IRuntimeModule() {};
            // 初始化模块
            virtual int Initialize() = 0;
            // 模块结束，打扫战场用
            virtual void Finalize() = 0;
            // 模块执行，每次进行一个单位的处理
            virtual void Tick() = 0;
    };
}