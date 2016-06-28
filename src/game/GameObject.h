#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#ifdef _MSC_VER
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#else
#include <stdint.h>
#endif

#include "Graphics.h"
#include "game/GameManager.h"
#include "NetworkManager.h"

enum GAMEOBJECTS{
	PLAYER, BULLET, SOLDIER76
};

class GameObject{
	public:
		GameObject(GameManager* m);

		friend bool operator< (const GameObject& g1, const GameObject& gp);
		virtual void display()=0;
		virtual void gameUpdate(Uint32 time)=0;
		virtual ~GameObject(){};

		//Includes the gamobject id
		virtual NetworkManager::Message serialize(char* buffer)=0;

		virtual void unserialize(NetworkManager::Message m)=0;


		//These functions are a reminder that all gameobjects
		//should have their own image loading functions
		static void init();
		static void del();
		uint32_t id;

	protected:
		GameManager* manager;
};

#endif
