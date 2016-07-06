#ifndef _RECTANGLEHITBOX_H
#define _RECTANGLEHITBOX_H
#include <math.h>
#include <limits>

#include "Graphics.h"
#include "game/Hitbox.h"

class RectangleHitbox : public Hitbox{
	public:
		
		//x and y are the center of the rectangle, and rotation is based around
		//that
		RectangleHitbox(float x=0, float y=0,
				float w=0, float h=0, float rotation=0);
		virtual ~RectangleHitbox();
		virtual bool intersectsWith(Hitbox *h);
		Hitbox::Point vertex[4];

	protected:
		float x;
		float y;
		float w;
		float h;
		float rotation;

		//Order of vertexes are
		//  3      0
		//
		//
		//  2      1
		//before rotation

		BoundingBox getBoundingBox();
		void setRotation(float r);

	private:
		void updatePointArray();

		//Intersection code
		static void projectRectangleBox(RectangleHitbox* r, Vector ax,
				float* min_out, float* max_out);
		inline bool intersect(float min1, float max1,
				float min2, float max2);
		void getAxes(Vector* v);
};

#endif
