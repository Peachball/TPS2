#pragma once
#include <iostream>
#include "game/GameObject.h"
#include "game/Player.h"
#include <vector>
#include <thread>

class GameObject;

class GameManager{
	public:
		enum State{
			BEFORE,
			DURING,
			PAUSED,
			END
		};

		GameManager();
		~GameManager();

		void startGame();

		//Stops all thread associated with game
		void endGame();

		int status;
	private:
		std::vector<GameObject*> objects;

		void render();
		void renderloop();
		void eventHandler();
		void manageGame();

		std::thread* eventThread;
		std::thread* gameThread;

		Player* localPlayer;
};
