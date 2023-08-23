#pragma once
#include "EngineFactory.h"
#include "RefCntAutoPtr.hpp"
#include "RenderDevice.h"
#include "DeviceContext.h"
#include "SwapChain.h"

namespace universe 
{
    class AppBase {
    public: 
        virtual ~AppBase() {}

        virtual void update(double curTime, double elapsedTime) {};

        virtual void render() = 0;

        virtual void present() = 0;

        virtual void windowResize(int width, int height) = 0; 

        virtual bool isInitialized() { return mInitialized;};

    protected:
        bool mInitialized = false;

        Diligent::IEngineFactory*  mEngineFactory = nullptr;
        Diligent::IRenderDevice*   mDevice = nullptr;
        Diligent::IDeviceContext** mPContexts = nullptr;
        Diligent::Uint32           mNumImmediateCtx = 1;
        Diligent::Uint32           mNumDeferredCtx = 0;
    };
}