#pragma once
#include <GLFW/glfw3.h>

class keyboard
{
public:
	static void glfw_keycallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	static bool key_pressed(int keycode);
	static bool key_released(int keycode);
	static bool key_changed(int keycode);
	static bool key_down(int keycode);
private:
	static bool m_active_keys[];
	static bool m_changed_keys[];
};