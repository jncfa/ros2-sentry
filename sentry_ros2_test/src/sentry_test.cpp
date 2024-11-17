#include <sentry.h>
#include <cstring>
int main(void) {
  sentry_options_t *options = sentry_options_new();
  sentry_options_set_dsn(options, "https://9527a7b54370e34942f523ba74ff9b69@o4508315891662848.ingest.de.sentry.io/4508316055371856");
  // This is also the default-path. For further information and recommendations:
  // https://docs.sentry.io/platforms/native/configuration/options/#database-path
  sentry_options_set_database_path(options, ".sentry-native");
  sentry_options_set_release(options, "sentry_ros2_test@2.3.12");
  sentry_options_set_debug(options, 1);
  sentry_options_set_handler_path(options, "/workspaces/ros2-sentry/install/opt/sentry_vendor/bin/crashpad_handler");
  sentry_init(options);

  /* ... */
  memset((void*)(intptr_t)4, 123, 1);

  // make sure everything flushes
  sentry_close();
}
