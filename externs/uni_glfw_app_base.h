#pragma once

#include "uni_app_base.h"
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
        GLFWAppBase();

        ~GLFWAppBase();

        virtual void initialize(void* winHandle, RenderMode mode);

        virtual void deinitialize();

    private:
        GLFWwindow* mWindow;
    };
}