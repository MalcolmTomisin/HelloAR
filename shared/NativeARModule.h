#pragma once

#include <ARSpecsJSI.h>

#include <memory>
#include <string>
#include <jsi/jsi.h>

#include "IPlatformServices.h"

namespace facebook::react {

class NativeARModule : public NativeARModuleCxxSpec<NativeARModule> {
    std::unique_ptr<IPlatformServices> platformServices_;

public:
  NativeARModule(std::shared_ptr<CallInvoker> jsInvoker, std::unique_ptr<IPlatformServices> platformServices);

  ~NativeARModule();

  bool initARCore(jsi::Runtime& rt, std::string rnPlatform);

};

} // namespace facebook::react
