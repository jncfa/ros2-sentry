cmake_minimum_required(VERSION 3.8)
project(opentelemetry_cpp_vendor)

find_package(ament_cmake REQUIRED)
find_package(ament_cmake_vendor_package REQUIRED)

include("opentelemetry_cpp_vendor-extras.cmake")
find_package(opentelemetry-cpp CONFIG QUIET)

option(WITH_PROMETHEUS "Indicate if you want to build with prometheus exporter" OFF)
option(WITH_OTLP_HTTP "Indicate if you want to build with OTLP HTTP exporter" ON)
option(WITH_OTLP_GRPC "Indicate if you want to build with OTLP GRPC" ON)
option(WITH_ASYNC_EXPORT_PREVIEW "Whether to enable async export" ON)

ament_vendor(opentelemetry_cpp_vendor
  SATISFIED ${opentelemetry_cpp_FOUND}
  VCS_URL https://github.com/open-telemetry/opentelemetry-cpp.git
  VCS_VERSION v1.19.0
  CMAKE_ARGS
  -DWITH_PROMETHEUS=${WITH_PROMETHEUS}
  -DWITH_OTLP_HTTP=${WITH_OTLP_HTTP}
  -DWITH_OTLP_GRPC=${WITH_OTLP_GRPC}
  -DWITH_ASYNC_EXPORT_PREVIEW=${WITH_ASYNC_EXPORT_PREVIEW}
    # all of these are required for building dynlibs
  -DOPENTELEMETRY_INSTALL=ON
  -DBUILD_SHARED_LIBS=ON
  -DCMAKE_POSITION_INDEPENDENT_CODE=ON
)

# export dependency to avoid having to do a target_include_directories
# on packages that include this one
ament_export_dependencies(opentelemetry-cpp)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  list(APPEND AMENT_LINT_AUTO_EXCLUDE
    ament_cmake_cppcheck
  )
  ament_lint_auto_find_test_dependencies()
endif()

install(DIRECTORY cmake DESTINATION share/${PROJECT_NAME})

ament_package(
  CONFIG_EXTRAS "opentelemetry_cpp_vendor-extras.cmake"
)