#include "somethingidk.hpp"

namespace d7gfx {
    namespace priv {
        namespace somethingidk {
            std::atomic<bool> ledPatternRunning(false), consoleTop(false), consoleBottom(false);
            bool socuInitialized = false, curlInitialized = false;
            Handle ptmsysmHandle = 0;
            u32* socubuf = nullptr;
        } /* somethingidk */
    } /* priv */
} /* d7gfx */