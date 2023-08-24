#pragma once
#include "EngineFactory.h"
#include "RefCntAutoPtr.hpp"
#include "RenderDevice.h"
#include "DeviceContext.h"
#include "SwapChain.h"
#include <vector>

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

        Diligent::RefCntAutoPtr<Diligent::IEngineFactory>  mEngineFactory;
        Diligent::RefCntAutoPtr<Diligent::IRenderDevice>   mDevice;
        std::vector<Diligent::RefCntAutoPtr<Diligent::IDeviceContext>> mPContexts;
        Diligent::RefCntAutoPtr<Diligent::IDeviceContext>  mPImmediateContext;
        Diligent::Uint32           mNumImmediateCtx = 1;
        Diligent::Uint32           mNumDeferredCtx = 0;
        Diligent::RefCntAutoPtr<Diligent::ISwapChain>                  mSwapChain;
    };
}