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
	private:

		static const int SOLDATA_SIZE=0;
		static const std::string DEFAULT_NAME;
		static const int IMG_HEIGHT=255;
		static const int IMG_WIDTH=323;
};


#endif
