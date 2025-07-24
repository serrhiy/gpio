add_library(
  gpio
  Chip.cc
  ChipInfo.cc
  LineInfo.cc
  InfoEvent.cc
  LineSettings.cc
  LineConfig.cc
  RequestConfig.cc
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBGPIOD REQUIRED IMPORTED_TARGET libgpiod)
target_link_libraries(gpio PUBLIC PkgConfig::LIBGPIOD)
