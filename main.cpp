#include <iostream>
#include <vector>
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>


#include "Graphics/GraphicsEngineOpenGL/interface/EngineFactoryOpenGL.h"
#include "Graphics/GraphicsEngine/interface/RenderDevice.h"
#include "Graphics/GraphicsEngine/interface/DeviceContext.h"
#include "Graphics/GraphicsEngine/interface/SwapChain.h"
#include "Common/interface/RefCntAutoPtr.hpp"

using namespace Diligent;

void glfw_error_callback(int error, const char* description)
{
    std::cout << "glfw report error code: " << error << " with description: " << description << std::endl;
}

int main()
{
    std::cout << "Hello UniEngine" << std::endl;
    GLFWwindow* window  = nullptr;

    //Initialize and terminating GLFW
    if(!glfwInit())
    {
        std::cout << "glfw failed to intialize!" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //set error callback, this will receive reports when error occurs like a key being pressed, window moved etc.
    glfwSetErrorCallback(glfw_error_callback);

    //creating a window and context
    window = glfwCreateWindow(640, 480, "UniEngine", NULL, NULL);
    if(!window)
    {
        std::cout << "glfw failed to create a window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    void* windowHandle = nullptr;
    windowHandle = static_cast<void*>(glfwGetCocoaWindow(window));
    NativeWindow nw {windowHandle};



    //Intialize Diligent 
    SwapChainDesc scDesc;

    //RENDER_DEVICE_TYPE renderDeviceType = RENDER_DEVICE_TYPE_GL;
    EngineGLCreateInfo engineCI;
    engineCI.Window = nw;
    engineCI.NumDeferredContexts = 0;
    std::vector<IDeviceContext*> ppContexts;
    RefCntAutoPtr<IEngineFactory> engineFactory;
    RefCntAutoPtr<ISwapChain> swapChain;
    RefCntAutoPtr<IRenderDevice> device;

    auto* pFactoryOpenGL = GetEngineFactoryOpenGL();
    engineFactory = pFactoryOpenGL;

    ppContexts.resize(1 + engineCI.NumDeferredContexts);
    pFactoryOpenGL->CreateDeviceAndSwapChainGL(engineCI, &device, ppContexts.data(), scDesc, &swapChain);



    //glClearColor( 0.4f, 0.3f, 0.4f, 0.0f );

    //main loop
    while(!glfwWindowShouldClose(window))
    {
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    //destroy window
     glfwDestroyWindow(window);

    //terminal glfw
    glfwTerminate();

    return 0;
}