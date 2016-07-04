#ifndef _SOLDIER76_H
#define _SOLDIER76_H
#include "game/Player.h"

class Soldier76 : public Player{
	public:
		Soldier76(GameManager* m, float x=400, float y=400);
		virtual void display();
		virtual void gameUpdate(Uint32 time);
		virtual ~Soldier76();
		static void init();
		static void del();

		virtual NetworkManager::Message serialize(char* buffer);
		virtual void unserialize(NetworkManager::Message m);

		void displayUI();
	private:

		SDL_RendererFlip flip;

		//Networking
		static const int SOLDATA_SIZE=0;

		//Sprite
		static const std::string DEFAULT_NAME;
		static const int IMG_HEIGHT=255;
		static const int IMG_WIDTH=323;

		//Health bar
		static const int HP_HEIGHT;
		static const int HP_DIST;
};


#endif
