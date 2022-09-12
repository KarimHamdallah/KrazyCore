#pragma once
#include <GLFW/glfw3.h>

class mouse
{
public:
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	static bool button_pressed(int button);
	static bool button_released(int button);
	static bool button_changed(int button);
	static bool button_down(int button);

	inline static double get_xpos() { return m_x_pos; }
	inline static double get_ypos() { return m_y_pos; }

	inline static double get_scrollx() { return m_scrollx; }
	inline static double get_scrolly() { return m_scrolly; }

	inline static double get_dx() { double ret = m_dx; m_dx = 0.0f; return ret; }
	inline static double get_dy() { double ret = m_dy; m_dy = 0.0f; return ret; }
private:
	static bool m_active_buttons[];
	static bool m_changed_buttons[];

	static double m_x_pos, m_y_pos;
	static double m_scrollx, m_scrolly;
	static double m_last_x, m_last_y;
	static double m_dx, m_dy;
	static bool m_first_mouse;
};