#ifndef _GAMEMANAGER_H
#define _GAMEMANAGER_H
#include <algorithm>
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
		void setLocalPlayer(Player* p);
	private:
		std::list<GameObject*> objects;

		void render();
		void manageGame();

		std::thread* gameThread;

		Player* localPlayer;
};

#endif
