cmake_minimum_required(VERSION 3.8)
project(otel_ros2_test)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(opentelemetry_cpp_vendor REQUIRED)
find_package(opentelemetry-cpp REQUIRED CONFIG)

add_executable(otel_test src/otel_test.cpp)
target_include_directories(otel_test PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)
target_compile_features(otel_test PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17
ament_target_dependencies(
  otel_test
  "opentelemetry_cpp_vendor"
)

target_link_libraries(
  otel_test
  opentelemetry-cpp::api
  opentelemetry-cpp::sdk
  opentelemetry-cpp::logs
  opentelemetry-cpp::trace
  opentelemetry-cpp::ostream_span_exporter
  opentelemetry-cpp::ostream_log_record_exporter
)

install(TARGETS otel_test
  DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
