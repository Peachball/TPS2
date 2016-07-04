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

		virtual NetworkManager::Message serialize(char* buffer);
		virtual void unserialize(NetworkManager::Message m);

		static void init();
		static void del();

	private:
		static const int BULLETDATA_SIZE=13;
		static std::mutex imageLock;
		static SDL_Texture *image;
		static SDL_Surface* surface;
		static std::string IMAGE_LOC;
		static const int BULLET_IMG_HEIGHT = 39;
		static const int BULLET_IMG_WIDTH = 120;
		static const float BULLET_SPEED;

		SDL_Rect src;
		SDL_Rect dest;
		float speed;
		float xpos;
		float ypos;

		float direction;

};


#endif
