#include "GameObject.h"

GameObject::GameObject(GameManager* m){
	manager = m;
}

void GameObject::init(){
}

void GameObject::del(){
}

bool operator< (const GameObject& g1, const GameObject& gp){
	return gp.id < g1.id;
}
