CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

SET(ProjectName "plotcpp")

PROJECT(${ProjectName})

set(CMAKE_CXX_STANDARD 11)

find_file(GNUPLOT_EXECUTABLE "wgnuplot.exe")
configure_file(plot_defines.h.in plot_defines.h)

set(SRCS plot_gnuplot.cpp)
set(HDRS plot_gnuplot.h)


add_library(${ProjectName} ${SRCS} ${HDRS})
target_include_directories(${ProjectName}  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} PUBLIC ${CMAKE_CURRENT_BINARY_DIR} )

add_executable(example example.cpp)
target_link_libraries(example PRIVATE ${ProjectName})


enable_testing()
add_executable(test_plotcpp
    test.cpp)

target_link_libraries(test_plotcpp PRIVATE ${ProjectName} )

add_test(test01 test_plotcpp test01)
add_test(test02 test_plotcpp test02)