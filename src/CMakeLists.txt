add_executable(main main.cc)
target_link_libraries(main PRIVATE gpio)
target_include_directories(main PRIVATE ../lib)