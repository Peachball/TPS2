#pragma once
#include "GameManager.h"
#include "Screen.h"

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
		GameManager manager;
		GAMEMODE gm;
};
