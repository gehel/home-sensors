cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/arduino-cmake/cmake/ArduinoToolchain.cmake)
set(PROJECT_NAME collector)
project(${PROJECT_NAME})

set(UIPEthernet_RECURSE True)

set(${CMAKE_PROJECT_NAME}_SKETCH collector.ino)
set(${CMAKE_PROJECT_NAME}_PORT /dev/ttyUSB0)
set(${CMAKE_PROJECT_NAME}_BOARD nano328)
set(${CMAKE_PROJECT_NAME}_SERIAL picocom @SERIAL_PORT@ -b 57600 -l)

generate_arduino_firmware(${CMAKE_PROJECT_NAME})
