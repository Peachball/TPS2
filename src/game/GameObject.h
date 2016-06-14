#pragma once
#include "Graphics.h"

class GameObject{
	public:
		virtual void display()=0;
		virtual void gameUpdate(float time)=0;
		
		virtual ~GameObject(){};
};
