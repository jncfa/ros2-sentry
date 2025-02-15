cmake_minimum_required(VERSION 3.8)
project(sentry_ros2_test)

function(separate_release_and_debug TARGET)
    # NOTE: The dbg extension is used solely because it is used as a standard
    # NOTE: In Linux documentation etc.
    # NOTE: Obtained from https://www.man7.org/linux/man-pages/man1/objcopy.1.html
    add_custom_command(
        TARGET ${TARGET}
        # Separate the Debug Information from the Target file
        COMMAND ${CMAKE_OBJCOPY} ARGS --only-keep-debug $<TARGET_FILE:${TARGET}> $<TARGET_FILE:${TARGET}>.dbg
        # As we have previously separated the debug information from the 
        # previous output, remove all debug information from the same
        COMMAND ${CMAKE_OBJCOPY} ARGS --strip-debug $<TARGET_FILE:${TARGET}>
        # Link the Separate Debug Information with the Final Release
        COMMAND ${CMAKE_OBJCOPY} ARGS --add-gnu-debuglink=$<TARGET_FILE:${TARGET}>.dbg $<TARGET_FILE:${TARGET}>
        DEPENDS ${TARGET}
        POST_BUILD
    )
    # NOTE: Add this to the clean target
    # NOTE: This is needed because otherwise old debug information
    # NOTE:  Will not be cleared upon clean
    set_property(
        TARGET ${TARGET}
        APPEND
            PROPERTY 
                ADDITIONAL_CLEAN_FILES 
                    $<TARGET_FILE:${TARGET}>.dbg

    )
endfunction(separate_release_and_debug)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(sentry_vendor REQUIRED)
find_package(sentry REQUIRED CONFIG)

get_executable_path(CRASHPAD_HANDLER sentry_crashpad::crashpad_handler BUILD)

add_executable(sentry_test src/sentry_test.cpp)
target_include_directories(sentry_test PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)
target_compile_features(sentry_test PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17
add_compile_definitions(CRASHPAD_HANDLER_PATH="${CRASHPAD_HANDLER}")

ament_target_dependencies(
  sentry_test
  "sentry_vendor"
  sentry
)


separate_release_and_debug(sentry_test)

install(TARGETS sentry_test
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
