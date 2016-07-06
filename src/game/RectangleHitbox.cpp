#include "RectangleHitbox.h"

RectangleHitbox::RectangleHitbox(
		float x, float y, float w, float h, float rotation
		){
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->rotation = -rotation;

	updatePointArray();
}

void RectangleHitbox::setRotation(float r){
	rotation = r;
}

bool RectangleHitbox::intersectsWith(Hitbox* h){
	{
		if(RectangleHitbox* r = dynamic_cast<RectangleHitbox*>(h)){
			Hitbox::Vector v[4];
			getAxes(v);
			for(int i = 0; i < 4; i++){
				Hitbox::Vector vp = v[i];
				float min1, min2, max1, max2;
				projectRectangleBox(this, vp, &min1, &max1);
				projectRectangleBox(r, vp, &min2, &max2);
				if(intersect(min1, min2, max1, max2)){
					return false;
				}
			}

			return true;
		}
	}
	logError("Unknown hitbox type");
	return false;
}


void RectangleHitbox::projectRectangleBox(RectangleHitbox* r, Hitbox::Vector ax,
		float* min_out, float* max_out){
	*max_out = 0;
	*min_out = std::numeric_limits<float>::max();
	for(int i = 0; i < 4; i++){
		float proj = vector[i].x * ax.x + vector[i].y * ax.y;
		if(proj < *min_out){
			*min_out = proj;
		}
		if(proj > *max_out){
			*max_out = proj;
		}
	}
}

inline bool RectangleHitbox::intersect(float min1, float max1, float min2, float max2){
	if((max1 < max2 && max1 > min2) || (max2 > min1 && max2 < max1)){
		return false;
	}
	return true;
}

Hitbox::BoundingBox RectangleHitbox::getBoundingBox(){
	Hitbox::BoundingBox b;


	return b;
}

void RectangleHitbox::getAxes(Hitbox::Vector* v){
	for(int i = 0; i < 4; i++){
		//modulo 4 because rectangles only have 4 vertices
		v[i].x = vertex[i].x - vertex[(i % 4)].x;
		v[i].y = vertex[i].y - vertex[(i % 4)].y;
	}
}

void RectangleHitbox::updatePointArray(){
	vertex[1].x = x + ((w/2.0f) * cos(rotation))
		- ((h/2.0f) * sin(rotation));
	vertex[0].x = x + ((w/2.0f) * cos(rotation))
		+ ((h/2.0f) * sin(rotation));
	vertex[2].x = x - ((w/2.0f) * cos(rotation))
		- ((h/2.0f) * sin(rotation));
	vertex[3].x = x - ((w/2.0f) * cos(rotation))
		+ ((h/2.0f) * sin(rotation));

	vertex[1].y = y + ((w/2.0f) * sin(rotation))
		+ ((h/2.0f) * cos(rotation));
	vertex[0].y = y + ((w/2.0f) * sin(rotation))
		- ((h/2.0f) * cos(rotation));
	vertex[2].y = y - ((w/2.0f) * sin(rotation))
		+ ((h/2.0f) * cos(rotation));
	vertex[3].y = y - ((w/2.0f) * sin(rotation))
		- ((h/2.0f) * cos(rotation));
}

RectangleHitbox::~RectangleHitbox(){

}
