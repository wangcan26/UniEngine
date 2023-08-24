#include "uni_glfw_app_base.h"
#include <GLFW/glfw3native.h>
#include "NanoLog.hpp"
#include "EngineFactoryOpenGL.h"

namespace universe 
{
    void glfw_error_callback(int error, const char* description)
    {
        LOG_INFO << "glfw report error code: " << error << " with description: " << description;
    }
    GLFWAppBase::GLFWAppBase()
    {
        LOG_INFO << "GLFWAppBase constructor";
    }

    GLFWAppBase::~GLFWAppBase()
    {
        LOG_INFO << "GLFWAppBase destructor";
    }

    void GLFWAppBase::initialize(CreateInfo& createInfo)
    {
        RenderMode mode = createInfo.renderMode;
        int width = createInfo.width;
        int height = createInfo.height;
        int backendMinV = createInfo.minorVersion;
        int backendMaxV = createInfo.majorVersion;
        char* title = createInfo.title;
        mVsync = createInfo.vSync;
        LOG_INFO << "GLFWAppBase initialize info <<<title: " << title << " width: " << width 
            << " height: " << height << " minV: " << backendMinV << " maxV: " << backendMaxV
            << " vSync: " << mVsync; 
        //Initialize and terminating GLFW
        if(!glfwInit())
        {
            LOG_INFO << "GLFWAppBase failed to intialize glfw";
            return;
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, backendMaxV);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, backendMinV);

        if(mode == RenderMode::OpenGL)
        {
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        }else {
            LOG_INFO << "GLFWAppBase do not support other graphics backend than OpenGL";
            return;
        }
        //set error callback, this will receive reports when error occurs like a key being pressed, window moved etc.
        glfwSetErrorCallback(glfw_error_callback);

        //creating a window and context
        mWindow = glfwCreateWindow(width, height, title, NULL, NULL);
        if(!mWindow)
        {
            LOG_INFO << "GLFWAppBase failed to intialize glfw";
            glfwTerminate();
            return ;
        }

        glfwMakeContextCurrent(mWindow);

        void* windowHandle = nullptr;
        windowHandle = static_cast<void*>(glfwGetCocoaWindow(mWindow));
        Diligent::NativeWindow nw {windowHandle};

        //Intialize Diligent 
        Diligent::SwapChainDesc scDesc;
        std::vector<Diligent::IDeviceContext*> ppContexts;
        Diligent::RefCntAutoPtr<Diligent::IEngineFactory> engineFactory;
        Diligent::RefCntAutoPtr<Diligent::ISwapChain>  swapChain;
        Diligent::RefCntAutoPtr<Diligent::IRenderDevice> device;

#if UNI_PLATFORM_MACOS
        scDesc.BufferCount = 3;
#endif 
        int numImmediateContext = 1;
        if(mode == RenderMode::OpenGL)
        {
            auto* pFactoryOpenGL =  Diligent::GetEngineFactoryOpenGL();
            engineFactory = pFactoryOpenGL;
            Diligent::EngineGLCreateInfo engineCI;
            engineCI.Window = nw;
            engineCI.NumDeferredContexts = mNumDeferredCtx;
            ppContexts.resize(numImmediateContext + engineCI.NumDeferredContexts);
            pFactoryOpenGL->CreateDeviceAndSwapChainGL(engineCI, &device, ppContexts.data(), scDesc, &swapChain);
        }

        if(!device)
        {
            LOG_INFO << "GLFWAppBase failed to create render swapchain and device";
            return;
        }


        //Initialize Sample
        mSample = createSample();
        if(!mSample)
        {
            LOG_INFO << "GLFWAppBase failed to create sample";
            return;
        }
        mNumImmediateCtx = numImmediateContext;
        mPContexts.resize(ppContexts.size());
        for(size_t i = 0; i < ppContexts.size(); ++i)
        {
            mPContexts[i].Attach(ppContexts[i]);
        }
        

        mDevice = device;
        mEngineFactory = engineFactory;
        mSwapChain = swapChain;
        mPImmediateContext = mPContexts[0];
        mNumDeferredCtx = static_cast<Diligent::Uint32>(mPContexts.size()) - mNumImmediateCtx;
        SampleBase::SampleInitInfo sampleInitInfo {
            .pEngineFactory = mEngineFactory,
            .pRenderDevice = mDevice,
            .pDeviceContext = mPImmediateContext,
            .pSwapChain = mSwapChain,
            .numImmediateCtx = mNumImmediateCtx,
            .numDefferredCtx = mNumDeferredCtx
        };
        mSample->initialize(sampleInitInfo);
        mInitialized = true;
    }

    void GLFWAppBase::deinitialize()
    {
        if(!isInitialized())
        {
            return;
        }

        if(mSample)
        {
            mSample->deinitialize();
            delete mSample;
        }
        if(!mPContexts.empty())
        {
            for(Diligent::Uint32 q = 0; q < mNumImmediateCtx; ++q)
            {
                mPContexts[q]->Flush();
            }
            mPContexts.clear();
        }
        mNumImmediateCtx = 0;
        mSwapChain.Release();
        mDevice.Release();

        //destroy window
         glfwDestroyWindow(mWindow);

        //terminal glfw
        glfwTerminate();
    }

    bool GLFWAppBase::isInitialized()
    {
        return mInitialized;
    }

    bool GLFWAppBase::isRunning()
    {
        if(!isInitialized())
        {
            return false;
        }

        if(!glfwWindowShouldClose(mWindow))
        {
            /* Swap front and back buffers */
            glfwSwapBuffers(mWindow);

            /* Poll for and process events */
            glfwPollEvents();
            return true;
        }
        return false;
    }

    void GLFWAppBase::present()
    {
        if(!mSwapChain)
        {
            return;
        }
#if UNI_PLATFORM_MACOS
        //do nothing
#endif 
    }
}