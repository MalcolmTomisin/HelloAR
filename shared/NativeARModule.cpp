#include "NativeARModule.h"

namespace facebook::react {

NativeARModule::NativeARModule(std::shared_ptr<CallInvoker> jsInvoker, std::unique_ptr<IPlatformServices> platformServices)
    : NativeARModuleCxxSpec(std::move(jsInvoker)),
        platformServices_(std::move(platformServices))
     {}

NativeARModule::~NativeARModule() {}

bool NativeARModule::initARCore(jsi::Runtime& rt, std::string rnPlatform) {
  return platformServices_->isGooglePlayServicesAvailable();
}

} // namespace facebook::react