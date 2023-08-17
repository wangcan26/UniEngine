#include <iostream>
#include <GLFW/glfw3.h>

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