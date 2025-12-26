#pragma once

#include <fbjni/fbjni.h>
#include <string>
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

    void onSurfaceCreated(jint reactTag, facebook::jni::alias_ref<facebook::jni::JObject> surface)
    {
        if (helloARApplication_)
        {
            JNIEnv *env = facebook::jni::Environment::current();
            helloARApplication_->OnSurfaceCreated(env, reactTag, surface.get());
        }
    }

    void onSurfaceChanged(jint reactTag, jint width, jint height)
    {
        if (helloARApplication_)
        {
            helloARApplication_->OnSurfaceChanged(reactTag, width, height);
        }
    }

    void onSurfaceDestroyed(jint reactTag)
    {
        if (helloARApplication_)
        {
            helloARApplication_->OnSurfaceDestroyed(reactTag);
        }
    }

    void onGlSurfaceCreated(jint reactTag)
    {
        if (helloARApplication_)
        {
            helloARApplication_->OnGlSurfaceCreated(reactTag);
        }
    }

    void onGlSurfaceChanged(jint reactTag, jint width, jint height)
    {
        if (helloARApplication_)
        {
            helloARApplication_->OnGlSurfaceChanged(reactTag, width, height);
        }
    }

    void onGlDrawFrame(jint reactTag)
    {
        if (helloARApplication_)
        {
            helloARApplication_->OnGlDrawFrame(reactTag);
        }
    }

    void onGestureTap(jint reactTag, jfloat x, jfloat y)
    {
        if (helloARApplication_)
        {
            helloARApplication_->OnGestureTap(reactTag, x, y);
        }
    }

    void setPaused(jint reactTag, jboolean paused)
    {
        if (helloARApplication_)
        {
            helloARApplication_->SetPaused(reactTag, paused);
        }
    }

    void setPlaneDetectionEnabled(jint reactTag, jboolean enabled)
    {
        if (helloARApplication_)
        {
            helloARApplication_->SetPlaneDetectionEnabled(reactTag, enabled);
        }
    }

    void setLightEstimationEnabled(jint reactTag, jboolean enabled)
    {
        if (helloARApplication_)
        {
            helloARApplication_->SetLightEstimationEnabled(reactTag, enabled);
        }
    }

    void setSessionType(jint reactTag, facebook::jni::alias_ref<facebook::jni::JString> value)
    {
        if (helloARApplication_)
        {
            helloARApplication_->SetSessionType(reactTag, value->toStdString());
        }
    }

    void setDepthMode(jint reactTag, facebook::jni::alias_ref<facebook::jni::JString> value)
    {
        if (helloARApplication_)
        {
            helloARApplication_->SetDepthMode(reactTag, value->toStdString());
        }
    }

    void setCloudAnchorMode(jint reactTag, facebook::jni::alias_ref<facebook::jni::JString> value)
    {
        if (helloARApplication_)
        {
            helloARApplication_->SetCloudAnchorMode(reactTag, value->toStdString());
        }
    }

    void setInstantPlacementMode(jint reactTag, facebook::jni::alias_ref<facebook::jni::JString> value)
    {
        if (helloARApplication_)
        {
            helloARApplication_->SetInstantPlacementMode(reactTag, value->toStdString());
        }
    }

    void setLightEstimationMode(jint reactTag, facebook::jni::alias_ref<facebook::jni::JString> value)
    {
        if (helloARApplication_)
        {
            helloARApplication_->SetLightEstimationMode(reactTag, value->toStdString());
        }
    }

    void setPlaneDetectionMode(jint reactTag, facebook::jni::alias_ref<facebook::jni::JString> value)
    {
        if (helloARApplication_)
        {
            helloARApplication_->SetPlaneDetectionMode(reactTag, value->toStdString());
        }
    }

    void setFocusMode(jint reactTag, facebook::jni::alias_ref<facebook::jni::JString> value)
    {
        if (helloARApplication_)
        {
            helloARApplication_->SetFocusMode(reactTag, value->toStdString());
        }
    }

    void setDebugShowPlanes(jint reactTag, jboolean enabled)
    {
        if (helloARApplication_)
        {
            helloARApplication_->SetDebugShowPlanes(reactTag, enabled);
        }
    }

    void setDebugShowPointCloud(jint reactTag, jboolean enabled)
    {
        if (helloARApplication_)
        {
            helloARApplication_->SetDebugShowPointCloud(reactTag, enabled);
        }
    }

    void setDebugShowWorldOrigin(jint reactTag, jboolean enabled)
    {
        if (helloARApplication_)
        {
            helloARApplication_->SetDebugShowWorldOrigin(reactTag, enabled);
        }
    }

    void setDebugShowDepthMap(jint reactTag, jboolean enabled)
    {
        if (helloARApplication_)
        {
            helloARApplication_->SetDebugShowDepthMap(reactTag, enabled);
        }
    }

    void onARViewMounted()
    {
        if (helloARApplication_)
        {
            helloARApplication_->OnARViewMounted();
        }
    }

    void onARViewUnmounted()
    {
        if (helloARApplication_)
        {
            helloARApplication_->OnARViewUnmounted();
        }
    }

    void destroySession()
    {
        if (helloARApplication_)
        {
            helloARApplication_->DestroySession();
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
            makeNativeMethod(
                "onSurfaceCreated",
                "(ILandroid/view/Surface;)V",
                HelloARApplicationWrapper::onSurfaceCreated),
            makeNativeMethod(
                "onSurfaceChanged",
                "(III)V",
                HelloARApplicationWrapper::onSurfaceChanged),
            makeNativeMethod(
                "onSurfaceDestroyed",
                "(I)V",
                HelloARApplicationWrapper::onSurfaceDestroyed),
            makeNativeMethod(
                "onGlSurfaceCreated",
                "(I)V",
                HelloARApplicationWrapper::onGlSurfaceCreated),
            makeNativeMethod(
                "onGlSurfaceChanged",
                "(III)V",
                HelloARApplicationWrapper::onGlSurfaceChanged),
            makeNativeMethod(
                "onGlDrawFrame",
                "(I)V",
                HelloARApplicationWrapper::onGlDrawFrame),
            makeNativeMethod(
                "onGestureTap",
                "(IFF)V",
                HelloARApplicationWrapper::onGestureTap),
            makeNativeMethod(
                "setPaused",
                "(IZ)V",
                HelloARApplicationWrapper::setPaused),
            makeNativeMethod(
                "setPlaneDetectionEnabled",
                "(IZ)V",
                HelloARApplicationWrapper::setPlaneDetectionEnabled),
            makeNativeMethod(
                "setLightEstimationEnabled",
                "(IZ)V",
                HelloARApplicationWrapper::setLightEstimationEnabled),
            makeNativeMethod(
                "setSessionType",
                "(ILjava/lang/String;)V",
                HelloARApplicationWrapper::setSessionType),
            makeNativeMethod(
                "setDepthMode",
                "(ILjava/lang/String;)V",
                HelloARApplicationWrapper::setDepthMode),
            makeNativeMethod(
                "setCloudAnchorMode",
                "(ILjava/lang/String;)V",
                HelloARApplicationWrapper::setCloudAnchorMode),
            makeNativeMethod(
                "setInstantPlacementMode",
                "(ILjava/lang/String;)V",
                HelloARApplicationWrapper::setInstantPlacementMode),
            makeNativeMethod(
                "setLightEstimationMode",
                "(ILjava/lang/String;)V",
                HelloARApplicationWrapper::setLightEstimationMode),
            makeNativeMethod(
                "setPlaneDetectionMode",
                "(ILjava/lang/String;)V",
                HelloARApplicationWrapper::setPlaneDetectionMode),
            makeNativeMethod(
                "setFocusMode",
                "(ILjava/lang/String;)V",
                HelloARApplicationWrapper::setFocusMode),
            makeNativeMethod(
                "setDebugShowPlanes",
                "(IZ)V",
                HelloARApplicationWrapper::setDebugShowPlanes),
            makeNativeMethod(
                "setDebugShowPointCloud",
                "(IZ)V",
                HelloARApplicationWrapper::setDebugShowPointCloud),
            makeNativeMethod(
                "setDebugShowWorldOrigin",
                "(IZ)V",
                HelloARApplicationWrapper::setDebugShowWorldOrigin),
            makeNativeMethod(
                "setDebugShowDepthMap",
                "(IZ)V",
                HelloARApplicationWrapper::setDebugShowDepthMap),

            makeNativeMethod(
                "onARViewMounted",
                "()V",
                HelloARApplicationWrapper::onARViewMounted),
            makeNativeMethod(
                "onARViewUnmounted",
                "()V",
                HelloARApplicationWrapper::onARViewUnmounted),
            makeNativeMethod(
                "destroySession",
                "()V",
                HelloARApplicationWrapper::destroySession),
        });
    }
};