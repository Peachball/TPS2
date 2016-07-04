#ifndef _PLAYER_H
#define _PLAYER_H
#define _USE_MATH_DEFINES
#include <string.h>
#include <math.h>
#include "game/GameObject.h"
#include "Graphics.h"
#include <math.h>
#include <SDL2/SDL.h>
#include "game/Bullet.h"

class Player : public GameObject{
	public:
		enum PlayerKeys{
			UP, DOWN, RIGHT, LEFT, SHOOT_BUTTON
		};
		Player(GameManager* m, float x=400, float y=400);
		virtual void display();
		virtual ~Player();
		void getInput();
		void getInput(NetworkManager::Message m);
		NetworkManager::Message serializeInput(char* loc);

		virtual void gameUpdate(Uint32 time);

		virtual NetworkManager::Message serialize(char* buffer);
		virtual void unserialize(NetworkManager::Message m);

		static void init();
		static void del();
		bool localPlayer=false;

		static const int KEYS=100;
		static const int PLAYERDATA_SIZE = 13;
		static const int PLAYER_INPUT_SIZE = KEYS + 8; //Data consists of keys, and 2 ints

	protected:
		int hp;
		float xpos;
		float ypos;
		static SDL_Texture *image;
		bool* keystate;
		SDL_Rect src;
		SDL_Rect dest;

		int mouse_x;
		int mouse_y;
		float movementSpeed = 0.6;

	private:
		static const std::string DEFAULT_NAME;

		void shoot(float direction);

};

#endif
