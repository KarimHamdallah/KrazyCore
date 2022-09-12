#include "keyboard.h"


bool keyboard::m_active_keys[GLFW_KEY_LAST];
bool keyboard::m_changed_keys[GLFW_KEY_LAST];

void keyboard::glfw_keycallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	if(action != GLFW_RELEASE)
	{
		if(!m_active_keys[key])
		{
			// to avoid any extra key out of array
			if (key <= (int)GLFW_KEY_LAST)
				m_active_keys[key] = true;
		}
	}
	else
	{
		m_active_keys[key] = false;
	}

	m_changed_keys[key] = (action != GLFW_REPEAT);
}

bool keyboard::key_pressed(int keycode)
{
	return m_active_keys[keycode];
}

bool keyboard::key_released(int keycode)
{
	return key_changed(keycode) && !m_active_keys[keycode];
}

bool keyboard::key_changed(int keycode)
{
	bool ret = m_changed_keys[keycode];
	m_changed_keys[keycode] = false;
	return ret;
}

bool keyboard::key_down(int keycode)
{
	return key_changed(keycode) && m_active_keys[keycode];
}