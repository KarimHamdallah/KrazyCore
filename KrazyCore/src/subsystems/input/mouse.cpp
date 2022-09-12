#include "mouse.h"

bool mouse::m_active_buttons[GLFW_MOUSE_BUTTON_LAST];
bool mouse::m_changed_buttons[GLFW_MOUSE_BUTTON_LAST];

double mouse::m_x_pos = 0.0;
double mouse::m_y_pos = 0.0;
double mouse::m_scrollx = 0.0;
double mouse::m_scrolly = 0.0;
double mouse::m_last_x = 0.0;
double mouse::m_last_y = 0.0;
double mouse::m_dx = 0.0;
double mouse::m_dy = 0.0;
bool mouse::m_first_mouse = true;

void mouse::mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
	if (action != GLFW_RELEASE)
	{
		if (!m_active_buttons[button])
		{
			// to avoid any extra button out of array
			if(button <= (int)GLFW_MOUSE_BUTTON_LAST)
				m_active_buttons[button] = true;
		}
	}
	else
	{
		m_active_buttons[button] = false;
	}

	m_changed_buttons[button] = (action != GLFW_REPEAT);
}

void mouse::cursor_position_callback(GLFWwindow * window, double xpos, double ypos)
{
	m_x_pos = xpos;
	m_y_pos = ypos;

	if (m_first_mouse)
	{
		m_last_x = xpos;
		m_last_y = ypos;
		m_first_mouse = false;
	}

	m_dx = m_x_pos - m_last_x;
	m_dy = m_y_pos - m_last_y;

	m_last_x = m_x_pos;
	m_last_y = m_y_pos;
}

void mouse::scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
	m_scrollx = xoffset;
	m_scrolly = yoffset;
}

bool mouse::button_pressed(int button)
{
	return m_active_buttons[button];
}

bool mouse::button_released(int button)
{
	return button_changed(button) && !m_active_buttons[button];
}

bool mouse::button_changed(int button)
{
	bool ret = m_changed_buttons[button];
	m_changed_buttons[button] = false;
	return ret;
}

bool mouse::button_down(int button)
{
	return button_changed(button) && m_active_buttons[button];
}
