#include <glad/glad.h>
#include "window.h"
#include "subsystems/asserts.h"
#include "subsystems/input/keyboard.h"
#include "subsystems/input/mouse.h"
#include <core/Application.h>

u32 window::m_width = 0;
u32 window::m_height = 0;

window::window(std::string title, u32 width, u32 height)
	: m_glfw_window(nullptr),m_title(title.c_str())
{
	m_width = width;
	m_height = height;
	init();
}

window::~window()
{
	shutdown();
}

void window::update()
{
	glfwPollEvents();
	glfwSwapBuffers(m_glfw_window);
}

void window::init()
{
	KRAZY_ASSERT_MSG(glfwInit(), "Failed to initialize GLFW!...contact engine developer");

	m_glfw_window = glfwCreateWindow(m_width, m_height, m_title, nullptr, nullptr);

	KRAZY_ASSERT_MSG(m_glfw_window, "Failed to creat GLFW window!...contact engine developer");

	glfwMakeContextCurrent(m_glfw_window);

	KRAZY_ASSERT_MSG(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to initialize Glad!...contact engine developer");

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	// setup input callbacks
	glfwSetKeyCallback(m_glfw_window, keyboard::glfw_keycallback);
	glfwSetMouseButtonCallback(m_glfw_window, mouse::mouse_button_callback);
	glfwSetCursorPosCallback(m_glfw_window, mouse::cursor_position_callback);
	glfwSetScrollCallback(m_glfw_window, mouse::scroll_callback);

	glfwSetWindowSizeCallback(m_glfw_window, window_size_callback);

	glfwSwapInterval(1);
}

void window::shutdown()
{
	glfwDestroyWindow(m_glfw_window);
	glfwTerminate();
}

void window::window_size_callback(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);

	Application::get_Instance()->set_width(width);
	Application::get_Instance()->set_height(height);
}
