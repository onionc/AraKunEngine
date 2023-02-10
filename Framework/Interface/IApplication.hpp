#pragma once
#include "Interface.hpp"
#include "IRuntimeModule.hpp"

// 用于抽象化不同平台的Application，并将其模块化，使我们可以用同一个入口main.c启动程序（也包含同一套启动参数）
namespace My{
    Interface IApplication : implements IRuntimeModule
    {
        public:
            virtual int Initialize() = 0;
            virtual void Finalize() = 0;
            virtual void Tick() = 0;

            virtual bool IsQuit() = 0;
    }
}