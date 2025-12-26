#include "utils.h"

namespace helloar
{
    namespace utils
    {

        void ThrowJavaException(JNIEnv *env, const char *msg)
        {
            LOGE("Throw Java exception: %s", msg);
            jclass c = env->FindClass("java/lang/RuntimeException");
            env->ThrowNew(c, msg);
        }
    }
}