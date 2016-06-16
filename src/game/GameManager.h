#pragma once
#include <iostream>
#include <vector>
#include <thread>

#include "Graphics.h"

//Include files are in cpp file
class GameObject;
class Player;

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
