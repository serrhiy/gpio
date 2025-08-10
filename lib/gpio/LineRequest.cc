#include "LineRequest.hh"
#include "LineInfo.hh"

#include <gpiod.h>
#include <stdexcept>
#include <ranges>

namespace gpio {

  bool LineRequest::isValid() const { return line_request != nullptr; }

  void LineRequest::throwIfIsNotValid() const {
    if (!isValid()) {
      throw std::runtime_error{ "Line request object is not valid" };
    }
  }

  void LineRequest::release() {
    if (!isValid()) return;
    gpiod_line_request_release(line_request);
    line_request = nullptr;
  }

  LineRequest::LineRequest(gpiod_line_request* line_request)
    : line_request{ line_request } {
    if (!isValid()) {
      throw std::runtime_error{ "Cannot allocate Line Request" };
    }
  }

  LineRequest::LineRequest(LineRequest&& other) noexcept
    : line_request{ other.line_request } {
    other.line_request = nullptr;
  }

  LineRequest& LineRequest::operator=(LineRequest&& other) {
    if (this != &other) {
      release();
      line_request = other.line_request;
      other.line_request = nullptr;
    }
    return *this;
  }

  LineRequest::~LineRequest() { release(); }

  std::string LineRequest::getChipName() const {
    throwIfIsNotValid();
    return gpiod_line_request_get_chip_name(line_request);
  }

  size_t LineRequest::getNumRequestedLines() const {
    throwIfIsNotValid();
    return gpiod_line_request_get_num_requested_lines(line_request);
  }

  std::vector<unsigned> LineRequest::getRequestedOffsets() const {
    const size_t num_offsets = getNumRequestedLines();
    std::vector<unsigned> offsets(num_offsets);
    gpiod_line_request_get_requested_offsets(line_request, offsets.data(), num_offsets);
    return offsets;
  }

  LineValue LineRequest::getValue(unsigned offset) const {
    throwIfIsNotValid();

    gpiod_line_value line_value = gpiod_line_request_get_value(line_request, offset);
    return line_value_map.at(line_value);
  }

  std::map<unsigned, LineValue> LineRequest::getValuesSubset(const std::vector<unsigned int>& offsets) const {
    throwIfIsNotValid();

    std::vector<gpiod_line_value> values(offsets.size());
    gpiod_line_request_get_values_subset(line_request, offsets.size(), offsets.data(), values.data());
    std::map<unsigned, LineValue> result;
    for (int index = 0; index < offsets.size(); index++) {
      result[offsets[index]] = line_value_map.at(values[index]);
    }
    return result;
  }

  std::map<unsigned, LineValue> LineRequest::getValues() const {
    return getValuesSubset(getRequestedOffsets());
  }

  void LineRequest::setValue(unsigned offset, LineValue value) const {
    int result = gpiod_line_request_set_value(line_request, offset, line_value_reverse_map.at(value));
    if (result == -1) {
      throw std::runtime_error{ "setValue error: " + std::to_string(result) };
    }
  }

  void LineRequest::setValuesSubset(const std::map<unsigned, LineValue>& data) const {
    auto keys = data | std::views::keys;
    auto values = data
      | std::views::values
      | std::views::transform([](const auto& value) { return line_value_reverse_map.at(value); });

    std::vector<unsigned> offsets(keys.begin(), keys.end());
    std::vector<gpiod_line_value> line_values(values.begin(), values.end());
    int result = gpiod_line_request_set_values_subset(line_request, data.size(), offsets.data(), line_values.data());
    if (result == -1) {
      throw std::runtime_error{ "setValuesSubset error: " + std::to_string(result) };
    }
  }

  void LineRequest::setValues(std::vector<LineValue>& values) const {
    std::vector<unsigned> offsets = getRequestedOffsets();
    if (offsets.size() != values.size()) {
      throw std::runtime_error{ "Invalid size of values" };
    }
    std::map<unsigned, LineValue> data;
    for (int index = 0; index < offsets.size(); index++) {
      data[offsets[index]] = values[index];
    }
    setValuesSubset(data);
  }

  void LineRequest::reconfigureLines(const LineConfig& line_config) const {
    int result = gpiod_line_request_reconfigure_lines(line_request, line_config.line_config);
    if (result == -1) {
      const auto message = "reconfigureLines error: " + std::to_string(result);
      throw std::runtime_error{ message };
    }
  }

  int LineRequest::getFd() const {
    return gpiod_line_request_get_fd(line_request);
  }

  int LineRequest::waitEdgeEvents(int64_t timeout_ns) const {
    return gpiod_line_request_wait_edge_events(line_request, timeout_ns);
  }

  
}
