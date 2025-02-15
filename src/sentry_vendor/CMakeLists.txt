cmake_minimum_required(VERSION 3.8)
project(sentry_vendor)

find_package(ament_cmake REQUIRED)
find_package(ament_cmake_vendor_package REQUIRED)

ament_add_default_options()

list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")
find_package(sentry QUIET)

ament_vendor(sentry_vendor
  VCS_URL https://github.com/getsentry/sentry-native.git
  SATISFIED ${sentry_FOUND}
  VCS_VERSION 0.7.12
  CMAKE_ARGS
    "-DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}"
    #-DCRASHPAD_ENABLE_STACKTRACE=ON
    "-DSENTRY_TRANSPORT_COMPRESSION=ON"
)

ament_export_dependencies(sentry)

install(DIRECTORY cmake DESTINATION share/${PROJECT_NAME})

ament_package(
  CONFIG_EXTRAS "sentry_vendor-extras.cmake"
)
