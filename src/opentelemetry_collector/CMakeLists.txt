cmake_minimum_required(VERSION 3.8)
project(opentelemetry_collector)

# Function to detect OS and architecture
function(get_platform_info OS_NAME ARCH)
    if(WIN32)
        set(${OS_NAME} "windows" PARENT_SCOPE)
    elseif(APPLE)
        set(${OS_NAME} "darwin" PARENT_SCOPE)
    elseif(UNIX)
        set(${OS_NAME} "linux" PARENT_SCOPE)
    endif()

    if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
        set(${ARCH} "x86_64" PARENT_SCOPE)
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64")
        set(${ARCH} "arm64" PARENT_SCOPE)
    endif()
endfunction()

# find dependencies
find_package(ament_cmake REQUIRED)

# Get platform information
get_platform_info(OS_NAME ARCH)

# define version and corresponding checksums
set(OTEL_VERSION "0.119.0")
if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
  SET(OTEL_CHECKSUM 4ee77545daaad658f7282bff98704bc1f31107890e2a2e1d4b1fc31da4648111)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64")
  SET(OTEL_CHECKSUM c5f4480504a12d9445ef5c3d754bd2824a2ea5dbc516c0edd52bb05c3e72f1f7)
else()
  message(FATAL_ERROR "Unknown architecture: ${CMAKE_SYSTEM_PROCESSOR}")
endif()

set(FILE_NAME "otelcol-contrib_${OTEL_VERSION}_linux_${ARCH}.tar.gz")
set(BINARY_NAME "otelcol-contrib${CMAKE_EXECUTABLE_SUFFIX}")

set(DOWNLOAD_URL
"https://github.com/open-telemetry/opentelemetry-collector-releases/releases/download/v${OTEL_VERSION}/${FILE_NAME}"
)

include(ExternalProject)
ExternalProject_Add(external-otelcol-contrib
URL               ${DOWNLOAD_URL}
URL_HASH          SHA256=${OTEL_CHECKSUM}
CONFIGURE_COMMAND ""
BUILD_COMMAND     ""
INSTALL_COMMAND   ${CMAKE_COMMAND} -E copy_if_different <SOURCE_DIR>/${BINARY_NAME} ${CMAKE_BINARY_DIR}/${BINARY_NAME}
BUILD_IN_SOURCE   TRUE # indicate we dont need a build dir
DOWNLOAD_EXTRACT_TIMESTAMP OLD
)

# Create an imported target
add_executable(otelcol-contrib IMPORTED GLOBAL)
add_dependencies(otelcol-contrib external-otelcol-contrib)

# Set the binary location
set_target_properties(otelcol-contrib PROPERTIES
    IMPORTED_LOCATION "${CMAKE_BINARY_DIR}/${BINARY_NAME}"
)

# export imported executable
install(IMPORTED_RUNTIME_ARTIFACTS otelcol-contrib RUNTIME DESTINATION lib/${PROJECT_NAME})
install(DIRECTORY launch DESTINATION share/${PROJECT_NAME})
install(DIRECTORY config DESTINATION share/${PROJECT_NAME})
ament_package()
