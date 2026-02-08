#pragma once

#include <fbjni/fbjni.h>
#include <string>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "HelloARApplication.h"
#include "AndroidPlatformServices.h"


class HelloARApplicationWrapper : public facebook::jni::HybridClass<HelloARApplicationWrapper>
{
public:
    static constexpr auto kJavaDescriptor = "Lcom/helloar/HelloAppSystem;";

    std::shared_ptr<helloar::HelloARApplication> helloARApplication_;

    explicit HelloARApplicationWrapper(AAssetManager *assetManager)
    {
        auto platformServices = std::make_unique<AndroidPlatformServices>();
        helloARApplication_ = std::make_shared<helloar::HelloARApplication>(
            std::move(platformServices),
            assetManager);
    }

    static facebook::jni::local_ref<jhybriddata> initHybrid(
        facebook::jni::alias_ref<jclass> /* clazz */,
        facebook::jni::alias_ref<facebook::jni::JObject> assetManager)
    {
        JNIEnv *env = facebook::jni::Environment::current();
        AAssetManager *nativeAssetManager = AAssetManager_fromJava(env, assetManager.get());
        return makeCxxInstance(nativeAssetManager);
    }

    void onPauseNative()
    {
        if (helloARApplication_)
        {
            helloARApplication_->OnPause();
        }
    }

    void onResumeNative(facebook::jni::alias_ref<facebook::jni::JObject> context, facebook::jni::alias_ref<facebook::jni::JObject> activity, jboolean hasCameraPermission)
    {
        if (helloARApplication_)
        {
            // Get the environment safely
            JNIEnv *env = facebook::jni::Environment::current();
            helloARApplication_->OnResume(env, context.get(), activity.get(), hasCameraPermission);
        }
    }

    void onSurfaceCreated()
    {
        if (helloARApplication_)
        {
            JNIEnv *env = facebook::jni::Environment::current();
            helloARApplication_->OnSurfaceCreated(env);
        }
    }

    void onSurfaceChanged(jint width, jint height)
    {
        if (helloARApplication_)
        {
            helloARApplication_->OnSurfaceChanged(width, height);
        }
    }

    void onSurfaceDestroyed()
    {
        if (helloARApplication_)
        {
            helloARApplication_->OnSurfaceDestroyed();
        }
    }

    void onGlSurfaceCreated()
    {
        if (helloARApplication_)
        {
            helloARApplication_->OnGlSurfaceCreated();
        }
    }

    void onGlSurfaceChanged(jint width, jint height)
    {
        if (helloARApplication_)
        {
            helloARApplication_->OnGlSurfaceChanged(width, height);
        }
    }

    void onGlDrawFrame()
    {
        if (helloARApplication_)
        {
            helloARApplication_->OnGlDrawFrame();
        }
    }

    void onGestureTap(jfloat x, jfloat y)
    {
        if (helloARApplication_)
        {
            helloARApplication_->OnGestureTap(x, y);
        }
    }

    void setPaused(jboolean paused)
    {
        if (helloARApplication_)
        {
            helloARApplication_->SetPaused(paused);
        }
    }

    void setPlaneDetectionEnabled(jboolean enabled)
    {
        if (helloARApplication_)
        {
            helloARApplication_->SetPlaneDetectionEnabled(enabled);
        }
    }

    void setLightEstimationEnabled(jboolean enabled)
    {
        if (helloARApplication_)
        {
            helloARApplication_->SetLightEstimationEnabled(enabled);
        }
    }

    void setSessionType(facebook::jni::alias_ref<facebook::jni::JString> value)
    {
        if (helloARApplication_)
        {
            helloARApplication_->SetSessionType(value->toStdString());
        }
    }

    void setDepthMode(facebook::jni::alias_ref<facebook::jni::JString> value)
    {
        if (helloARApplication_)
        {
            helloARApplication_->SetDepthMode(value->toStdString());
        }
    }

    void setCloudAnchorMode(facebook::jni::alias_ref<facebook::jni::JString> value)
    {
        if (helloARApplication_)
        {
            helloARApplication_->SetCloudAnchorMode(value->toStdString());
        }
    }

    void setInstantPlacementMode(facebook::jni::alias_ref<facebook::jni::JString> value)
    {
        if (helloARApplication_)
        {
            helloARApplication_->SetInstantPlacementMode(value->toStdString());
        }
    }

    void setLightEstimationMode(facebook::jni::alias_ref<facebook::jni::JString> value)
    {
        if (helloARApplication_)
        {
            helloARApplication_->SetLightEstimationMode(value->toStdString());
        }
    }

    void setPlaneDetectionMode(facebook::jni::alias_ref<facebook::jni::JString> value)
    {
        if (helloARApplication_)
        {
            helloARApplication_->SetPlaneDetectionMode(value->toStdString());
        }
    }

    void setFocusMode(facebook::jni::alias_ref<facebook::jni::JString> value)
    {
        if (helloARApplication_)
        {
            helloARApplication_->SetFocusMode(value->toStdString());
        }
    }

    void setDebugShowPlanes(jboolean enabled)
    {
        if (helloARApplication_)
        {
            helloARApplication_->SetDebugShowPlanes(enabled);
        }
    }

    void setDebugShowPointCloud(jboolean enabled)
    {
        if (helloARApplication_)
        {
            helloARApplication_->SetDebugShowPointCloud(enabled);
        }
    }

    void setDebugShowWorldOrigin(jboolean enabled)
    {
        if (helloARApplication_)
        {
            helloARApplication_->SetDebugShowWorldOrigin(enabled);
        }
    }

    void setDebugShowDepthMap(jboolean enabled)
    {
        if (helloARApplication_)
        {
            helloARApplication_->SetDebugShowDepthMap(enabled);
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
            makeNativeMethod(
                "initHybrid",
                "(Landroid/content/res/AssetManager;)Lcom/facebook/jni/HybridData;",
                HelloARApplicationWrapper::initHybrid),
            makeNativeMethod("onPauseNative", HelloARApplicationWrapper::onPauseNative),
            makeNativeMethod(
                "onResumeNative",
                "(Landroid/content/Context;Landroid/app/Activity;Z)V",
                HelloARApplicationWrapper::onResumeNative),
            makeNativeMethod(
                "onSurfaceCreated",
                "()V",
                HelloARApplicationWrapper::onSurfaceCreated),
            makeNativeMethod(
                "onSurfaceChanged",
                "(II)V",
                HelloARApplicationWrapper::onSurfaceChanged),
            makeNativeMethod(
                "onSurfaceDestroyed",
                "()V",
                HelloARApplicationWrapper::onSurfaceDestroyed),
            makeNativeMethod(
                "onGlSurfaceCreated",
                "()V",
                HelloARApplicationWrapper::onGlSurfaceCreated),
            makeNativeMethod(
                "onGlSurfaceChanged",
                "(II)V",
                HelloARApplicationWrapper::onGlSurfaceChanged),
            makeNativeMethod(
                "onGlDrawFrame",
                "()V",
                HelloARApplicationWrapper::onGlDrawFrame),
            makeNativeMethod(
                "onGestureTap",
                "(FF)V",
                HelloARApplicationWrapper::onGestureTap),
            makeNativeMethod(
                "setPaused",
                "(Z)V",
                HelloARApplicationWrapper::setPaused),
            makeNativeMethod(
                "setPlaneDetectionEnabled",
                "(Z)V",
                HelloARApplicationWrapper::setPlaneDetectionEnabled),
            makeNativeMethod(
                "setLightEstimationEnabled",
                "(Z)V",
                HelloARApplicationWrapper::setLightEstimationEnabled),
            makeNativeMethod(
                "setSessionType",
                "(Ljava/lang/String;)V",
                HelloARApplicationWrapper::setSessionType),
            makeNativeMethod(
                "setDepthMode",
                "(Ljava/lang/String;)V",
                HelloARApplicationWrapper::setDepthMode),
            makeNativeMethod(
                "setCloudAnchorMode",
                "(Ljava/lang/String;)V",
                HelloARApplicationWrapper::setCloudAnchorMode),
            makeNativeMethod(
                "setInstantPlacementMode",
                "(Ljava/lang/String;)V",
                HelloARApplicationWrapper::setInstantPlacementMode),
            makeNativeMethod(
                "setLightEstimationMode",
                "(Ljava/lang/String;)V",
                HelloARApplicationWrapper::setLightEstimationMode),
            makeNativeMethod(
                "setPlaneDetectionMode",
                "(Ljava/lang/String;)V",
                HelloARApplicationWrapper::setPlaneDetectionMode),
            makeNativeMethod(
                "setFocusMode",
                "(Ljava/lang/String;)V",
                HelloARApplicationWrapper::setFocusMode),
            makeNativeMethod(
                "setDebugShowPlanes",
                "(Z)V",
                HelloARApplicationWrapper::setDebugShowPlanes),
            makeNativeMethod(
                "setDebugShowPointCloud",
                "(Z)V",
                HelloARApplicationWrapper::setDebugShowPointCloud),
            makeNativeMethod(
                "setDebugShowWorldOrigin",
                "(Z)V",
                HelloARApplicationWrapper::setDebugShowWorldOrigin),
            makeNativeMethod(
                "setDebugShowDepthMap",
                "(Z)V",
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