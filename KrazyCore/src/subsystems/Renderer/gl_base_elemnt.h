#pragma once

#include "defines.h"

class gl_base_element
{
public:
	virtual void bind() = 0;
	virtual void unbind() = 0;
	virtual void delete_id() = 0;
	
	gl_id m_id;
protected:
};