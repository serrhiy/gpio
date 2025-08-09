#include "LineRequest.hh"

#include <gpiod.h>
#include <stdexcept>

namespace gpio {

  bool LineRequest::isValid() const {
    return line_request != nullptr;
  }

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

  LineRequest::LineRequest(gpiod_line_request* line_request):
    line_request{ line_request }
  {
    if (!isValid()) {
      throw std::runtime_error{ "Cannot allocate Line Request" };
    }
  }

  LineRequest::LineRequest(LineRequest&& other) noexcept:
    line_request{ other.line_request }
  {
    other.line_request = nullptr;
  }
  
  LineRequest &LineRequest::operator=(LineRequest&& other) {
    if (this != &other) {
      release();
      line_request = other.line_request;
      other.line_request = nullptr;
    }
    return *this;
  }

  LineRequest::~LineRequest() {
    release();
  }

  std::string LineRequest::getChipName() const {
    throwIfIsNotValid();
    return gpiod_line_request_get_chip_name(line_request);
  }


}
