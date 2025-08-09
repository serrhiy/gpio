#pragma once

#include <gpiod.h>
#include <string>

namespace gpio {
  class RequestConfig {
    gpiod_request_config* request_config;

    bool isValid() const;
    void throwIfIsNotValid() const;
    void free();

    public:
    explicit RequestConfig();

    RequestConfig(const RequestConfig&) = delete;
    RequestConfig& operator=(const RequestConfig&) = delete;

    RequestConfig(RequestConfig&& other) noexcept;
    RequestConfig& operator=(RequestConfig&& other);

    ~RequestConfig();

    void setConsumer(const std::string& consumer) const;
    std::string getConsumer() const;

    void setEventBufferSize(size_t size) const;
    size_t getEventBufferSize() const;
  };
}
