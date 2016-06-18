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

		int status;
	private:
		std::list<GameObject*> objects;

		void render();
		void eventHandler();
		void manageGame();

		std::thread* eventThread;
		std::thread* gameThread;

		Player* localPlayer;
};
