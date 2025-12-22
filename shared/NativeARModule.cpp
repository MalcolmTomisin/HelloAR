#include "NativeARModule.h"

namespace facebook::react {

NativeARModule::NativeARModule(std::shared_ptr<CallInvoker> jsInvoker, std::unique_ptr<IPlatformServices> platformServices)
    : NativeARModuleCxxSpec(std::move(jsInvoker)),
        platformServices_(std::move(platformServices))
     {}

NativeARModule::~NativeARModule() {}

bool NativeARModule::initARCore(jsi::Runtime& rt, jsi::String rnPlatform) {
  return platformServices_->isGooglePlayServicesAvailable();
}

bool NativeARModule::isDepthModeSupported(jsi::Runtime& rt) {
  // For simplicity, we assume depth mode is supported if Google Play Services is available
  return platformServices_->isGooglePlayServicesAvailable();
}

bool NativeARModule::isGeospatialModeSupported(jsi::Runtime& rt) {
  // For simplicity, we assume geospatial mode is supported if Google Play Services is available
  return platformServices_->isGooglePlayServicesAvailable();
}

} // namespace facebook::react