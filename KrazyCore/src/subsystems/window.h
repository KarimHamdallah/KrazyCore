#pragma once
#include <GLFW/glfw3.h>
#include <string>
#include "defines.h"


class window
{
public:
	window() = default;
	window(std::string title, u32 width, u32 height);
	~window();

	inline GLFWwindow* get_GLFW_window()const { return m_glfw_window; }
	bool is_running()const { return !glfwWindowShouldClose(m_glfw_window); }
	void set_running(bool shouldclose) { glfwSetWindowShouldClose(m_glfw_window, shouldclose); }

	inline static u32 get_width() { return m_width; };
	inline static u32 get_height() { return m_height; };
	inline const char* get_title()const { return m_title; };



	void update();

private:
	void init();
	void shutdown();

	static void window_size_callback(GLFWwindow* window, int width, int height);


	GLFWwindow* m_glfw_window;

	static u32 m_width;
	static u32 m_height;
	const char* m_title;

};