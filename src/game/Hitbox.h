#ifndef _HITBOX_H
#define _HITBOX_H
#include <functional>

#include "game/GameObject.h"

class Hitbox{
	public:
		struct BoundingBox{
			float x1;
			float x2;
			float y1;
			float y2;
		};

		struct Point{
			float x;
			float y;
		};

		struct Vector{
			float x;
			float y;
		};

		Hitbox(BoundingBox b);
		Hitbox();
		virtual ~Hitbox();
		BoundingBox box;

		virtual bool intersectsWith(Hitbox* h)=0;

	private:
		std::function<void()> handler;
		GameObject* g;
};

#endif
