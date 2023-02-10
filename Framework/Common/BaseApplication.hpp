#pragma once 

#include "IApplication.hpp"

namespace My{
    class BaseApplication: implements IApplication
    {
        public:
            virtual int Initialize();
            virtual void Finalize();
            virtual void Tick();
            virtual bool IsQuit();
        protected:
            // 退出主循环的标记
            bool m_bQuit;
    }
}