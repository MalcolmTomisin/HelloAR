// Utilities shared by native code.
// Keep this header strictly well-formed: if a namespace isn't closed here,
// it will leak into other includes and cause extremely confusing errors.

#pragma once

#include <jni.h>

#include <android/asset_manager.h>
#include <android/log.h>

#include <cstdlib>

#ifndef LOGI
#define LOGI(...) \
  __android_log_print(ANDROID_LOG_INFO, "hello_ar_example_c", __VA_ARGS__)
#endif  // LOGI

#ifndef LOGE
#define LOGE(...) \
  __android_log_print(ANDROID_LOG_ERROR, "hello_ar_example_c", __VA_ARGS__)
#endif  // LOGE

#ifndef CHECK
#define CHECK(condition)                                                   \
  if (!(condition)) {                                                      \
    LOGE("*** CHECK FAILED at %s:%d: %s", __FILE__, __LINE__, #condition); \
    abort();                                                               \
  }
#endif  // CHECK

#ifndef CHECKANDTHROW
#define CHECKANDTHROW(condition, env, msg, ...)                            \
  if (!(condition)) {                                                      \
    LOGE("*** CHECK FAILED at %s:%d: %s", __FILE__, __LINE__, #condition); \
    utils::ThrowJavaException(env, msg);                                    \
    return ##__VA_ARGS__;                                                  \
  }
#endif  // CHECKANDTHROW


namespace helloar {

    namespace utils {

    void ThrowJavaException(JNIEnv* env, const char* msg);
    }
}