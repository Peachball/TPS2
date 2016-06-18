#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H
#include "Graphics.h"
#include "game/GameManager.h"

class GameObject{
	public:
		GameObject(GameManager* m);

		virtual void display()=0;
		virtual void gameUpdate(Uint32 time)=0;
		virtual ~GameObject(){};

		static void init();
		static void del();

	protected:
		GameManager* manager;
};

#endif
