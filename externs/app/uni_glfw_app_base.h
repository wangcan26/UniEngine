#pragma once

#include "uni_app_base.h"
#include "uni_sample_base.h"
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3.h>
#include "Timer.hpp"

namespace universe
{
    class GLFWAppBase : public AppBase 
    {
        using AppBase::update;
    public:
        enum class RenderMode
        {
            OpenGL,
            MoltenVK,
            Metal
        };
        typedef struct CreateInfo
        {
            char* title;
            int width = 640;
            int height = 480;
            RenderMode renderMode = RenderMode::OpenGL;
            int majorVersion = 4;
            int minorVersion = 1;
            bool vSync = false;
        }CreateInfo;

        GLFWAppBase();

        ~GLFWAppBase();

        virtual void initialize(CreateInfo& createInfo);

        virtual void deinitialize();

        virtual bool isInitialized();

        virtual void update();

        virtual bool isRunning();

        void present() override;

    protected:
        GLFWwindow* mWindow;
        SampleBase* mSample = nullptr;
        bool        mVsync = false;
        Diligent::Timer  mTimer;
        double           mLastTime = 0.0;
    };
}