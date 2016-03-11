#pragma once
#include <iostream>
#include "GameObject.h"
#include "Player.h"
#include <vector>

class GameManager{
	public:
		GameManager();
		~GameManager();

		void startGame();
		void endGame();
	private:
		std::vector<GameObject*> objects;
};
