#ifndef _BULLET_H
#define _BULLET_H
#include <mutex>
#include <math.h>
#include "Graphics.h"
#include "game/GameObject.h"
#include "game/GameManager.h"

class Bullet : public GameObject{
	public:
		Bullet(GameManager* m, float x=0, float y=0, float direction=0);

		virtual void display();
		virtual void gameUpdate(Uint32 time);
		virtual ~Bullet();

		static void init();
		static void del();

	private:
		static std::mutex imageLock;
		static SDL_Texture *image;
		static std::string IMAGE_LOC;

		SDL_Rect src;
		SDL_Rect dest;
		float speed;
		float xpos;
		float ypos;

		float direction;

};


#endif