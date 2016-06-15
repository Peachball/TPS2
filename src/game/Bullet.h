#ifndef _BULLET_H
#define _BULLET_H
#include "game/GameObject.h"
#include "game/GameManager.h"
#include "Graphics.h"

class Bullet : public GameObject{
	public:
		Bullet(GameManager* m);
		virtual ~Bullet();
		virtual void gameUpdate(Uint32 time);
		virtual void display();

		static void init();

	private:
		static std::string BULLET_TEXTURE_LOC;
		static SDL_Texture *texture;
};


#endif
