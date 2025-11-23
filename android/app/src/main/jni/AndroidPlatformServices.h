#pragma once

#include <fbjni/fbjni.h>
#include "IPlatformServices.h"
#include "arcore_c_api.h"

class AndroidPlatformServices : public IPlatformServices {
public:

    bool isGooglePlayServicesAvailable() override;
    

private:
    ArSession* ar_session_ = nullptr;
    bool install_requested_ = false;
};