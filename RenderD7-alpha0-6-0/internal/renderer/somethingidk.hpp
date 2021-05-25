#pragma once
#include <3ds.h>
#include <atomic>

namespace d7gfx {
    namespace priv {
        namespace somethingidk {
            extern std::atomic<bool> ledPatternRunning, consoleTop, consoleBottom;
            extern bool socuInitialized, curlInitialized;
            extern Handle ptmsysmHandle;
            extern u32* socubuf;
        } /* somethingidk */
    } /* priv */
} /* d7gfx */