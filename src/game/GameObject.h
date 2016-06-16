#pragma once
#include "Graphics.h"
#include "game/GameManager.h"

class GameObject{
	public:
		GameObject(GameManager* m);

		virtual void display()=0;
		virtual void gameUpdate(Uint32 time)=0;
		virtual ~GameObject(){};

	protected:
		GameManager* manager;
};
