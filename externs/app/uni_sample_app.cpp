#include "uni_sample_app.h"
#include "NanoLog.hpp"

namespace universe
{
    SampleApp::SampleApp()
    {
        LOG_INFO << "SampleApp constructor";
    }

    SampleApp::~SampleApp()
    {
        LOG_INFO << "SampleApp deconstructor";
    }

    void SampleApp::initialize(CreateInfo& createInfo)
    {
        GLFWAppBase::initialize(createInfo);
    }

    void SampleApp::deinitialize()
    {
        GLFWAppBase::deinitialize();

    }

    void SampleApp::update(double curTime, double elapsedTime)
    {
        if(mDevice && mSample)
        {
            mSample->update(curTime, elapsedTime);
        }
    }

    void SampleApp::render()
    {
        if(mNumImmediateCtx == 0 || !mSwapChain)
        {
            return;
        }
        
        auto* pRTV = mSwapChain->GetCurrentBackBufferRTV();
        auto* pDSV = mSwapChain->GetDepthBufferDSV();
        mPImmediateContext->SetRenderTargets(1, &pRTV, pDSV, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        
        if(mSample)
        {
            mSample->render();
        }
        mPImmediateContext->SetRenderTargets(1, &pRTV, pDSV, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    }

    void SampleApp::windowResize(int width, int height)
    {
        if(!mSwapChain)
        {
            return;
        }
        mSwapChain->Resize(width, height);
    }
} 
