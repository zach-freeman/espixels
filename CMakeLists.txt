# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)
include(esp-idf/tools/cmake/project.cmake)
set(EXTRA_COMPONENT_DIRS externals/lv_port_esp32_ili9341/components)
project(hello-world)
