#pragma once

#include "LineConfig.hh"
#include "LineInfo.hh"

#include <gpiod.h>
#include <map>
#include <memory>
#include <ranges>
#include <string>
#include <vector>

namespace gpio {
  class LineRequest {
    gpiod_line_request* line_request;

    bool isValid() const;
    void throwIfIsNotValid() const;
    void release();
  public:
    LineRequest() = delete;
    explicit LineRequest(gpiod_line_request* line_request);

    LineRequest(const LineRequest&) = delete;
    LineRequest& operator=(const LineRequest&) = delete;

    LineRequest(LineRequest&& other) noexcept;
    LineRequest& operator=(LineRequest&& other);

    ~LineRequest();

    std::string getChipName() const;
    size_t getNumRequestedLines() const;
    std::vector<unsigned> getRequestedOffsets() const;
    LineValue getValue(unsigned offset) const;

    std::map<unsigned, LineValue>
      getValuesSubset(const std::vector<unsigned int>& offsets) const;

    std::map<unsigned, LineValue> getValues() const;

    void setValue(unsigned offset, LineValue value) const;
    void setValuesSubset(const std::map<unsigned, LineValue>& data) const;
    void setValues(std::vector<LineValue>& values) const;

    void reconfigureLines(const LineConfig& line_config) const;

    int getFd() const;

    int waitEdgeEvents(int64_t timeout_ns) const;
  };
}
