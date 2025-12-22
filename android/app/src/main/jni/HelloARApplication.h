#pragma once

#include <memory>
#include "ARSessionManager.h"
#include "AndroidPlatformServices.h"


namespace helloar
{
    class HelloARApplication
    {
    public:
        HelloARApplication(std::unique_ptr<AndroidPlatformServices> platformServices)
            : platformServices_(std::move(platformServices)) {}
        ~HelloARApplication(){
            ArSessionManager::Instance().Destroy();
        }
        void OnPause(){
            ArSessionManager::Instance().Pause();
        }
        void OnResume(JNIEnv* env, jobject context, jobject activity){
            if (!ArSessionManager::Instance().IsInitialized()) {
            auto canProceed = platformServices_->checkARCoreInstallation(env, context, activity);
            if (!canProceed) {
                return;
            }
            ArSessionManager::Instance().Create(env, context);
        }
        }

    private:
         std::unique_ptr<AndroidPlatformServices> platformServices_;
    };
}