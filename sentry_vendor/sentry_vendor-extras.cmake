list(APPEND CMAKE_MODULE_PATH "${sentry_vendor_DIR}/../../../opt/sentry_vendor/lib/sentry/cmake")
find_package(sentry REQUIRED)

list(APPEND sentry_vendor_TARGETS 
sentry::sentry
)

