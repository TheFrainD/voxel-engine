#include <vepch.h>
#include "Window.h"

// Headers
#include <Config.h>
#include <Core/Assert.h>
#include <Core/Log.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

static void glfw_error_callback(int error, const char* msg) {
    VE_LOG_ERROR("GLFW Error ({}): {}", error, msg);
}

namespace Voxel
{

    Window::Window(Uint32 width, Uint32 height, const std::string& title)
    {
        window = nullptr;
        
        data.width = width;
        data.height = height;
        data.title = title;

        Init();
    }

    Window::~Window()
    {
        Shutdown();
    }

    void Window::Init()
    {
        // Init GLFW
        int success = glfwInit();
        VE_ASSERT(success = GLFW_TRUE, "Failed to initialize GLFW!");
        glfwSetErrorCallback(glfw_error_callback);

        // Configure GLFW context
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	// set OpenGL version 3.3
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create windoww
        window = glfwCreateWindow(data.width, data.height, data.title.c_str(), nullptr, nullptr);
        VE_ASSERT(window != nullptr, "Failed to create window!");

        glfwMakeContextCurrent(window);
        glfwSetWindowUserPointer(window, &data);
        SetVSync(true);

        // Init glad
        success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        VE_ASSERT(success == GL_TRUE, "Failed to initialize glad!");

        glViewport(0, 0, data.width, data.height);
        
        glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
            WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
            data->width = width;
            data->height = height;

        });
    }

    void Window::Shutdown()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void Window::Update()
    {
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    void Window::SetVSync(bool vsync)
    {
        if (vsync)
        {
            glfwSwapInterval(1);
        }
        else
        {
            glfwSwapInterval(0);
        }

        data.vsync = vsync;
    }

} // namespace Voxel