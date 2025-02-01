#include <sentry.h>
#include <cstring>
#include <stdio.h>

void print_envelope(sentry_envelope_t *envelope, void *unused_state)
{
    (void)unused_state;
    size_t size_out = 0;
    char *s = sentry_envelope_serialize(envelope, &size_out);
    printf("%s", s);
    sentry_free(s);
    sentry_envelope_free(envelope);
}

int main(void) {
  sentry_options_t *options = sentry_options_new();
  sentry_options_set_dsn(options, "https://1f47648a187289af2453f90b97a75a98@o4508315891662848.ingest.de.sentry.io/4508723663536208");
  // This is also the default-path. For further information and recommendations:
  // https://docs.sentry.io/platforms/native/configuration/options/#database-path
  sentry_options_set_database_path(options, ".sentry-native");
  sentry_options_set_release(options, "native@0.0.1");
  sentry_options_set_debug(options, 1);
  sentry_options_set_handler_path(options, "/workspaces/ros2-sentry/install/opt/sentry_vendor/bin/crashpad_handler");

  sentry_init(options);

  memset((void*)(intptr_t)4, 123, 1);

  // make sure everything flushes
  sentry_close();
}
