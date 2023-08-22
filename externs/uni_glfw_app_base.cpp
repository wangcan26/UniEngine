#include "uni_glfw_app_base.h"
#include "NanoLog.hpp"

namespace universe 
{
    GLFWAppBase::GLFWAppBase()
    {
        LOG_INFO << "GLFWAppBase constructor";
    }

    GLFWAppBase::~GLFWAppBase()
    {
         LOG_INFO << "GLFWAppBase destructor";
    }

    void GLFWAppBase::initialize(void* winHandle, RenderMode mode)
    {

    }

    void GLFWAppBase::deinitialize()
    {

    }
}