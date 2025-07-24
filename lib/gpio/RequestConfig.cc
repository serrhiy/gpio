#include "RequestConfig.hh"

#include <gpiod.h>
#include <stdexcept>
#include <string>

namespace gpio {

  bool RequestConfig::isValid() const {
    return request_config != nullptr;
  }

  void RequestConfig::throwIfIsNotValid() const {
    if (!isValid()) {
      throw std::runtime_error{ "Request Config is not valid" };
    }
  }

  void RequestConfig::free() {
    if (!isValid()) return;
    gpiod_request_config_free(request_config);
    request_config = nullptr;
  }

  RequestConfig::RequestConfig()
    : request_config{ gpiod_request_config_new() }
  {
    if (!isValid()) {
      throw std::runtime_error{ "Cannot allocate Request Config" };
    }
  }

  RequestConfig::RequestConfig(RequestConfig&& other) noexcept:
    request_config{ other.request_config }
  {
    other.request_config = nullptr;
  }

  RequestConfig& RequestConfig::operator=(RequestConfig&& other) {
    if (this != &other) {
      free();
      request_config = other.request_config;
      other.request_config = nullptr;
    }
    return *this;
  }

  RequestConfig::~RequestConfig() {
    free();
  }

  void RequestConfig::setConsumer(const std::string& consumer) const {
    throwIfIsNotValid();
    gpiod_request_config_set_consumer(request_config, consumer.c_str());
  }

  std::string RequestConfig::getConsumer() const {
    throwIfIsNotValid();
    return gpiod_request_config_get_consumer(request_config);
  }

  void RequestConfig::setEventBufferSize(size_t size) const {
    throwIfIsNotValid();
    gpiod_request_config_set_event_buffer_size(request_config, size);
  }

  size_t RequestConfig::getEventBufferSize() const {
    throwIfIsNotValid();
    return gpiod_request_config_get_event_buffer_size(request_config);
  }
}
