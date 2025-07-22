#pragma once

#include <gpiod.h>
#include <unordered_map>

namespace gpio {
  enum class LineValue { ERROR, INACTIVE, ACTIVE };
  enum class LineDirection { AS_IS, INPUT, OUTPUT };
  enum class LineEdge { NONE, RISING, FALLING, BOTH };
  enum class LineBias { AS_IS, UNKNOWN, DISABLED, PULL_UP, PULL_DOWN };
  enum class LineDrive { PUSH_PULL, OPEN_DRAIN, OPEN_SOURCE };
  enum class LineClock { MONOTONIC, REALTIME, CLOCK_HTE };

  
}
