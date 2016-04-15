#pragma once
#include "Graphics.h"

class GameObject{
	public:
		virtual void display()=0;
		float xpos;
		float ypos;
};
