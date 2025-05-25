#include "SimpleEngineCore/Window.hpp"
#include "SimpleEngineCore/Log.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

namespace SimpleEngine {

    static bool s_GLFW_initialized = false;

    GLfloat point[] = {
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5, -0.5f, 0.0f
    };

    GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
    };

    const char* vertex_shader =
        "#version 460 core\n"
        "layout(location = 0) in vec3 vertex_position;"
        "layout(location = 1) in vec3 vertex_color;"
        "out vec3 color;"
        "void main() {"
        "color = vertex_color;"
        "gl_Position = vec4(vertex_position, 1.0);"
        "}";

    const char* fragment_shader =
        "#version 460 core\n"
        "in vec3 color;"
        "out vec4 frag_color;"
        "void main() {"
        "frag_color = vec4(color, 1.0);"
        "}";

    GLuint shader_program;
    GLuint vao;


	Window::Window(std::string title, const unsigned int width, const unsigned int height)
		: m_data({std::move(title), width, height})
	{
		int resultCote = init();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplOpenGL3_Init();
        ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	}

	Window::~Window() {
		shutdown();
	}

    int Window::init() {
        LOG_INFO("Creating window {0} width size {1}x{2}", m_data.title, m_data.width, m_data.height);

        if (!s_GLFW_initialized) {
            /* Initialize the library */
            if (!glfwInit()) {
                LOG_CRITICAL("Can't init GLFW");
                return -1;
            }
            s_GLFW_initialized = true;
        }

        /* Create a windowed mode window and its OpenGL context */
        m_window = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), NULL, NULL);
        if (!m_window)
        {
            LOG_CRITICAL("Can't create window {0}", m_data.title);
            return -2;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(m_window);


        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            LOG_CRITICAL("Failed to GLAD init");
            return -3;
        }

        glfwSetWindowUserPointer(m_window, &m_data);

        glfwSetWindowSizeCallback(m_window,
            [](GLFWwindow* pwindow, int width, int height) {
                LOG_INFO("New size {0}x{1}", width, height);

                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pwindow));
                data.width = width;
                data.height = height;

                EventWindowResize event(width, height);
                data.eventCallbackFn(event);
            }
        );

        glfwSetCursorPosCallback(m_window,
            [](GLFWwindow* pwindow, double x, double y) {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pwindow));
                EventMouseMoved event(x, y);
                data.eventCallbackFn(event);
            }
        );

        glfwSetWindowCloseCallback(m_window,
            [](GLFWwindow* pwindow) {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pwindow));
            
                EventWindowClose event;
                data.eventCallbackFn(event);
            }
        );

        glfwSetFramebufferSizeCallback(m_window,
            [](GLFWwindow* pwindow, int width, int height) {
                glViewport(0, 0, width, height);
            }
        );

        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vertex_shader, nullptr);
        glCompileShader(vs);

        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fragment_shader, nullptr);
        glCompileShader(fs);

        shader_program = glCreateProgram();
        glAttachShader(shader_program, vs);
        glAttachShader(shader_program, fs);
        glLinkProgram(shader_program);

        glDeleteShader(vs);
        glDeleteShader(fs);

        GLuint points_vbo = 0;
        glGenBuffers(1, &points_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);

        GLuint colors_vbo = 0;
        glGenBuffers(1, &colors_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);


        return 0;
    }

	void Window::shutdown() {
        glfwDestroyWindow(m_window);
        glfwTerminate();
	}

	void Window::on_update() {
        glClearColor(m_background_color[0], m_background_color[1], m_background_color[2], m_background_color[3]);
        glClear(GL_COLOR_BUFFER_BIT);


        glUseProgram(shader_program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);




        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize.x = static_cast<float>(get_width());
        io.DisplaySize.y = static_cast<float>(get_height());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        ImGui::Begin("Backgrounf Color Window");
        ImGui::ColorEdit4("Background Color", m_background_color);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_window);
        glfwPollEvents();  
    }
	
}