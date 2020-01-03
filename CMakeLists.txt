# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/main/Include/lv_conf.h ${CMAKE_CURRENT_SOURCE_DIR}/externals/lv_port_esp32_ili9341/components/lvgl/lv_conf.h)
include(esp-idf/tools/cmake/project.cmake)
set(EXTRA_COMPONENT_DIRS externals/lv_port_esp32_ili9341/components)
project(hello-world)
