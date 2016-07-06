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
	logError("Unknown hitbox type");
	return false;
}

Hitbox::BoundingBox RectangleHitbox::getBoundingBox(){
	Hitbox::BoundingBox b;


	return b;
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
