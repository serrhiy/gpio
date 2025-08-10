#include <iostream>

#include <gpio/utils.hh>
#include <logger/core.hh>
#include <thread>
#include <chrono>

namespace fs = std::filesystem;

int main(const int argc, const char* argv[]) {
  logger::open("ruspberry");

  while(true) {
    logger::logDebug("Awailable chips: {}", gpio::getChipsPath()[0].string());
    logger::logError("Awailable chips: {}", gpio::getChipsPath()[0].string());
    std::this_thread::sleep_for(std::chrono::seconds{ 1 });
  }
  return 0;
}
