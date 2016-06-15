#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H
#include "Graphics.h"

class GameManager;

class GameObject{
	public:

		GameObject(GameManager* m);
		virtual void display()=0;
		virtual void gameUpdate(Uint32 time)=0;
		virtual ~GameObject();

		//Load textures and such for the objects beforehand
		static void init();

	protected:
		GameManager* manager;
};

#endif
