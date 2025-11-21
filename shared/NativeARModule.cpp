#include "NativeARModule.h"

namespace facebook::react {

NativeARModule::NativeARModule(std::shared_ptr<CallInvoker> jsInvoker)
    : NativeARModuleCxxSpec(std::move(jsInvoker)) {}

bool NativeARModule::initARCore(jsi::Runtime& rt) {
  return true;
}

} // namespace facebook::react