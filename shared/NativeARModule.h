#pragma once

#include <ARSpecsJSI.h>

#include <memory>
#include <string>
#include <jsi/jsi.h>

namespace facebook::react {

class NativeARModule : public NativeARModuleCxxSpec<NativeARModule> {
public:
  NativeARModule(std::shared_ptr<CallInvoker> jsInvoker);

  bool initARCore(jsi::Runtime& rt);
};

} // namespace facebook::react
