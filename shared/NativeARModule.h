#pragma once

#include <ARSpecsJSI.h>

#include <memory>
#include <string>
#include <jsi/jsi.h>

#include "IPlatformServices.h"

namespace facebook::react {

class NativeARModule : public NativeARModuleCxxSpec<NativeARModule> {
private:
    std::unique_ptr<IPlatformServices> platformServices_;

public:
  NativeARModule(std::shared_ptr<CallInvoker> jsInvoker, std::unique_ptr<IPlatformServices> platformServices);

  ~NativeARModule();

  bool initARCore(jsi::Runtime& rt, jsi::String rnPlatform);
  bool isDepthModeSupported(jsi::Runtime& rt);
  bool isGeospatialModeSupported(jsi::Runtime& rt);

};

} // namespace facebook::react
