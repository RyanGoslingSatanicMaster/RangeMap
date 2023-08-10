

#include "InvalidRangeException.h"

namespace shock_audio{

    const char *InvalidRangeException::what() const noexcept {
        return "Invalid range was passed";
    }
}
