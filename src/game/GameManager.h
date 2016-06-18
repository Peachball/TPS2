#pragma once
#include <iostream>
#include <list>
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
		void endGame();

		void addObject(GameObject* g);
		void removeObject(GameObject* g);

		void handleEvents();

		int status;
	private:
		std::list<GameObject*> objects;

		void render();
		void manageGame();

		std::thread* gameThread;

		Player* localPlayer;
};
