#ifndef _BULLET_H
#define _BULLET_H
#include "Graphics.h"
#include "game/GameObject.h"
#include "game/GameManager.h"

class Bullet : public GameObject{
	public:
		Bullet(GameManager* m);

		virtual void display();
		virtual void gameUpdate(Uint32 time);
		virtual ~Bullet();

		static void init();
		static void del();

	private:
		static SDL_Texture *image;
		static std::string IMAGE_LOC;

};


#endif
