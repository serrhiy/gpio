#include <gpio/utils.hh>
#include <gpio/Chip.hh>
#include <gpio/ChipInfo.hh>
#include <gpio/LineSettings.hh>
#include <gpio/LineConfig.hh>
#include <gpio/RequestConfig.hh>
#include <logger/core.hh>

#include <thread>
#include <chrono>
#include <cmath>

namespace fs = std::filesystem;

bool approximatelyZero(const double x) {
  static constexpr double precision = 1e-4;
  return std::abs(x) <= precision;
}

double foo(const double x) {
  return std::sin(x);
}

void start() {
  constexpr unsigned line = 17;

  const auto chips_path = gpio::getDefaultChipPath();
  logger::logDebug("Chip file selected: {}", chips_path.string());

  const gpio::Chip chip{ chips_path };
  const gpio::ChipInfo chip_info = chip.getInfo();

  static const char* format = "Chip info. Name: {}, Label: {}, Num Lines: {}";
  logger::logDebug(format, chip_info.getName(), chip_info.getLabel(), chip_info.getNumLines());

  const gpio::LineSettings line_settings;
  line_settings.setDirection(gpio::LineDirection::OUTPUT);

  const gpio::LineConfig line_config;
  line_config.addLineSettings(line, line_settings);

  const gpio::RequestConfig request_config;
  const gpio::LineRequest line_request = chip.requestLines(request_config, line_config);

  while (true) {
    using namespace std::chrono;
    const duration time = system_clock::now().time_since_epoch();
    const double x = duration_cast<duration<double>>(time).count();
    if (approximatelyZero(1 - foo(x))) {
      line_request.setValue(line, gpio::LineValue::ACTIVE);
    }
    else if (approximatelyZero(1 + foo(x))) {
      line_request.setValue(line, gpio::LineValue::INACTIVE);
    }
    std::this_thread::yield();
  }
}

int main(const int argc, const char* argv[]) {
  try {
    logger::open("ruspberry");
    start();
  }
  catch(const std::exception& exception) {
    logger::logError(exception.what());
    return 1;
  }
  catch(...) {
    logger::logError("Unknown error occured");
    return 2;
  }
  return 0;
}
