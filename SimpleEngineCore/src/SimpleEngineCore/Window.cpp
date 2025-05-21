#include "SimpleEngineCore/Window.hpp"
#include "SimpleEngineCore/Log.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace SimpleEngine {

    static bool s_GLFW_initialized = false;

	Window::Window(std::string title, const unsigned int width, const unsigned int height)
		: m_title(std::move(title)), m_width{width}, m_height{height}
	{
		int resultCote = init();
	}

	Window::~Window() {
		shutdown();
	}

    int Window::init() {
        LOG_INFO("Creating window {0} width size {1}x{2}", m_title, m_width, m_height);

        if (!s_GLFW_initialized) {
            /* Initialize the library */
            if (!glfwInit()) {
                LOG_CRITICAL("Can't init GLFW");
                return -1;
            }
            s_GLFW_initialized = true;
        }

        /* Create a windowed mode window and its OpenGL context */
        m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), NULL, NULL);
        if (!m_window)
        {
            LOG_CRITICAL("Can't create window {0}", m_title);
            return -2;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(m_window);


        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            LOG_CRITICAL("Failed to GLAD init");
            return -3;
        }

        return 0;
    }

	void Window::shutdown() {
        glfwDestroyWindow(m_window);
        glfwTerminate();
	}

	void Window::on_update() {
        glClearColor(0.1f, 0.4f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(m_window);
        glfwPollEvents();  
    }
	
}