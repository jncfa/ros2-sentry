#include <cstring>
#include <cstdlib>
#include <cstdio>

#include <optional>
#include <string>
#include <string_view>
#include <filesystem>

#include <sentry.h>

template <class T>
T from_string(std::string_view str);

template <>
std::filesystem::path from_string(std::string_view str){
  return str;
}


// helper function to get environment variable as a optional
std::optional<std::string> getenv_opt(const std::string_view env)
{
  if (const auto* env_data = std::getenv(env.data()); env_data != nullptr){
    return std::string(env_data);
  }
  return std::nullopt;
}

std::string getenv_opt(const std::string_view env, const std::string& default_value)
{
  return getenv_opt(env).value_or(default_value);
}

template <class T>
T getenv_value(const std::string_view env, const T& default_value){
  auto const result = getenv_opt(env);
  if (result.has_value()){
    return from_string<T>(result->c_str());
  }
  return default_value;
}

std::filesystem::path get_default_path(){
  return getenv_value<std::filesystem::path>("XDG_CACHE_HOME", ".") / "my-app" / ".sentry-native";
}

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
  sentry_options_set_database_path(options, get_default_path().c_str());
  sentry_options_set_release(options, "native@0.0.1");
  sentry_options_set_debug(options, 1);
  sentry_options_set_handler_path(options, CRASHPAD_HANDLER_PATH);
  sentry_options_set_environment(options, "robot1");
  sentry_init(options);

  memset((void*)(intptr_t)4, 123, 1);

  // make sure everything flushes
  sentry_close();
}
