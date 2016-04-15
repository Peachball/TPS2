#pragma once
#include "GameManager.h"
#include "Screen.h"

enum GAMEMODE{
	SINGLEPLAYER,
	MULTIPLAYER,
	TESTER
};

class GameScreen: public Screen{
	public:
		GameScreen(GAMEMODE mode);
		virtual Screen* update();
		~GameScreen();

	private:
		GameManager manager;
		GAMEMODE gm;
};
