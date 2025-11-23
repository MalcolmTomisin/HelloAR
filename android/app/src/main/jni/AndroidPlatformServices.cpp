#include <cstdint>
#include <jni.h>
#include <fbjni/fbjni.h>

#include "AndroidPlatformServices.h"




bool AndroidPlatformServices::isGooglePlayServicesAvailable() {
    if (ar_session_ != nullptr){
        JNIEnv* env = facebook::jni::Environment::current();
        jclass activityThread = env->FindClass("android/app/ActivityThread");
        jmethodID currentActivityThread = env->GetStaticMethodID(activityThread, "currentActivityThread", "()Landroid/app/ActivityThread;");
        jobject activityThreadObj = env->CallStaticObjectMethod(activityThread, currentActivityThread);
    
        jmethodID getApplication = env->GetMethodID(activityThread, "getApplication", "()Landroid/app/Application;");
        jobject context = env->CallObjectMethod(activityThreadObj, getApplication);

        jclass trackerClass = env->FindClass("com/helloar/CurrentActivityTracker");
        if (trackerClass == NULL) {
            return false;
        }

        // 2. Get the static method ID for "getCurrentActivity"
        jmethodID getMethod = env->GetStaticMethodID(trackerClass, 
                                                        "getCurrentActivity", 
                                                        "()Landroid/app/Activity;");
        if (getMethod == NULL) {
            return false;
        }

        // 3. Call the method to get the Activity instance
        jobject activity = env->CallStaticObjectMethod(trackerClass, getMethod);
        ArInstallStatus install_status;
        bool user_requested_install = !install_requested_;
        ArAvailability availability;

        ArCoreApk_checkAvailability(env, context, &availability);

        switch(availability){
            case AR_AVAILABILITY_SUPPORTED_INSTALLED:
            case AR_AVAILABILITY_SUPPORTED_NOT_INSTALLED:
            case AR_AVAILABILITY_SUPPORTED_APK_TOO_OLD:
                break;
            case AR_AVAILABILITY_UNKNOWN_CHECKING:
                return false;
            default:
                return false;

        }

        ArStatus error = ArCoreApk_requestInstall(env, activity, user_requested_install , &install_status);
        if (error != AR_SUCCESS){
            return false;
        }

        switch(install_status){
            case AR_INSTALL_STATUS_INSTALLED:
                break;
            case AR_INSTALL_STATUS_INSTALL_REQUESTED:
                install_requested_ = true;
                return false;
        }

        error = ArSession_create(env, context, &ar_session_);
        if (error != AR_SUCCESS){
            return false;
        }
        return true;

        // int32_t is_supported = 0;
        // ArSession_isDepthModeSupported(ar_session_, AR_DEPTH_MODE_AUTOMATIC,
        //                                 &is_supported);
        // return is_supported;

    }
    return true;
}