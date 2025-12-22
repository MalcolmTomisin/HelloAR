#pragma once

#include <fbjni/fbjni.h>
#include "HelloARApplication.h"
#include "AndroidPlatformServices.h"

class HelloARApplicationWrapper : public facebook::jni::HybridClass<HelloARApplicationWrapper>
{
public:
    static constexpr auto kJavaDescriptor = "Lcom/helloar/HelloAppSystem;";

    std::shared_ptr<helloar::HelloARApplication> helloARApplication_;

    HelloARApplicationWrapper()
    {
        auto platformServices = std::make_unique<AndroidPlatformServices>();
        helloARApplication_ = std::make_shared<helloar::HelloARApplication>(std::move(platformServices));
    }

    static facebook::jni::local_ref<jhybriddata> initHybrid(
        facebook::jni::alias_ref<jclass> /* clazz */)
    {
        return makeCxxInstance();
    }

    void onPause()
    {
        if (helloARApplication_)
        {
            helloARApplication_->OnPause();
        }
    }

    void onResume(facebook::jni::alias_ref<facebook::jni::JObject> context, facebook::jni::alias_ref<facebook::jni::JObject> activity)
    {
        if (helloARApplication_)
        {
            // Get the environment safely
            JNIEnv *env = facebook::jni::Environment::current();
            helloARApplication_->OnResume(env, context.get(), activity.get());
        }
    }

    static void registerNatives()
    {
        registerHybrid({
            makeNativeMethod("initHybrid", HelloARApplicationWrapper::initHybrid),
            makeNativeMethod("onPause", HelloARApplicationWrapper::onPause),
            makeNativeMethod(
                "onResume",
                "(Landroid/content/Context;Landroid/app/Activity;)V",
                HelloARApplicationWrapper::onResume),
        });
    }
};