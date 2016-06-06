#pragma once
#include <iostream>
#include "GameObject.h"
#include "Player.h"
#include <vector>
#include <thread>

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
		void endGame();

		int status;
	private:
		std::vector<GameObject*> objects;
		void renderloop();
		void eventHandler();
		void manageGame();

		std::thread* renderThread;
		std::thread* eventThread;
		std::thread* gameThread;

		Player* localPlayer;
};
