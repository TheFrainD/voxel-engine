#pragma once

// Headers
#include <Config.h>
#include <Core/Log.h>

#ifdef VE_DEBUG

#ifdef VE_PLATFORM_WINDOWS
#define VE_DEBUGBREAK() __debugbreak()
#else
#define VE_DEBUGBREAK() __builtin_trap()
#endif // VE_PLATFORM_WINDOWS

#define VE_ASSERT(EXPR,  MSG) {\
    if (EXPR) {\
    } else {\
        VE_LOG_CRITICAL("Assertion failure: ({}), {} -- {}:{}", #EXPR, #MSG, __FILE__, __LINE__);\
        VE_DEBUGBREAK();\
    }\
}

#else

#define VE_ASSERT(EXPR, MSG)

#endif // VE_DEBUG