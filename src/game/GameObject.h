#pragma once
#include "Graphics.h"

class GameObject{
	public:
		virtual void display()=0;
		virtual void gameUpdate(Uint32 time)=0;
		
		virtual ~GameObject(){};
};
