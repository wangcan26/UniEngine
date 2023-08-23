#pragma once

#include "uni_app_base.h"
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3.h>

namespace universe
{
    class GLFWAppBase : public AppBase 
    {
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
        }CreateInfo;

        GLFWAppBase();

        ~GLFWAppBase();

        virtual void initialize(CreateInfo& createInfo);

        virtual void deinitialize();

        virtual bool isInitialized();

        virtual bool isRunning();

    private:
        GLFWwindow* mWindow;
    };
}