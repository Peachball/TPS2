#ifndef _GAMESCREEN_H
#define _GAMESCREEN_H
#include "GameManager.h"
#include "Screen.h"
#include "NetworkManager.h"
#include "Player.h"
#include "game/Soldier76.h"

enum GAMEMODE{
	SINGLEPLAYER,
	MULTIPLAYER_SERVER,
	MULTIPLAYER_CLIENT,
	TESTER
};

class GameScreen: public Screen{
	public:
		GameScreen(GAMEMODE mode);
		virtual Screen* update();
		virtual ~GameScreen();

	private:
		NetworkManager* net;
		GameManager manager;
		GAMEMODE gm;
};

#endif
