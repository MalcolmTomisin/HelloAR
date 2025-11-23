#pragma once
#include "IPlatformServices.h"

class AndroidPlatformServices : public IPlatformServices {
public:
    bool isGooglePlayServicesAvailable() override {
        // Here you would implement the actual check for Google Play Services availability
        // For demonstration purposes, we'll just return true
        return true;
    }
};