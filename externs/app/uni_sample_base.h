#pragma once 

#include "EngineFactory.h"
#include "RefCntAutoPtr.hpp"
#include "RenderDevice.h"
#include "DeviceContext.h"
#include "SwapChain.h"

namespace universe 
{
    class SampleBase 
    {
    public: 

        typedef struct SampleInitInfo {
            Diligent::IEngineFactory*  pEngineFactory = nullptr;
            Diligent::IRenderDevice*   pRenderDevice = nullptr;
            Diligent::IDeviceContext*  pDeviceContext = nullptr;
            Diligent::ISwapChain*      pSwapChain = nullptr;
            Diligent::Uint32           numImmediateCtx = 1;
            Diligent::Uint32           numDefferredCtx = 0;
        }SampleInitInfo;

        virtual ~SampleBase() {}

        virtual void initialize(SampleInitInfo& info) {
            mRenderDevice = info.pRenderDevice;
            mSwapChain = info.pSwapChain;
            mImmediateCtx = info.pDeviceContext;
        }

        virtual void deinitialize() = 0;

        virtual void update(double curTime, double elapsedTime)
        {

        }

        virtual void render() = 0;
        

    protected:
        Diligent::RefCntAutoPtr<Diligent::IRenderDevice>  mRenderDevice;
        Diligent::RefCntAutoPtr<Diligent::IDeviceContext> mImmediateCtx;
        Diligent::RefCntAutoPtr<Diligent::ISwapChain>     mSwapChain;
    };

    extern SampleBase* createSample();
};