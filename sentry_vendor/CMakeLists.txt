cmake_minimum_required(VERSION 3.8)
project(sentry_vendor)

find_package(ament_cmake REQUIRED)
find_package(ament_cmake_vendor_package REQUIRED)

ament_vendor(sentry_vendor
  VCS_URL https://github.com/getsentry/sentry-native.git
  VCS_VERSION 0.7.12
  CMAKE_ARGS
    "-DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}"
)

ament_export_dependencies(sentry)
ament_export_include_directories(${CMAKE_INSTALL_PREFIX}/opt/sentry_vendor/include)

ament_package(
  CONFIG_EXTRAS "sentry_vendor-extras.cmake"
)
