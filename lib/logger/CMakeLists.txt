add_library(logger INTERFACE)
find_package(fmt CONFIG REQUIRED)
target_link_libraries(logger INTERFACE fmt::fmt)