add_executable(main main.cc)

target_link_libraries(main PRIVATE gpio)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(main PRIVATE fmt::fmt)

target_include_directories(main PRIVATE ../lib)
