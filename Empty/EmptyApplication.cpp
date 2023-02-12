#include "BaseApplication.hpp"

namespace My{
    BaseApplication g_pApp;
    IApplication* g_pApp = &g_pApp;
}