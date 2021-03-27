#pragma once
#include <graphics/renderer_3d.h>
#include <box2d/box2d.h>
#include "game_object.h"

class Railing : public GameObject
{
public:
	Railing(gef::Vector4 position, gef::Vector4 scale, gef::Vector4 rotation);
	~Railing();

	void render(gef::Renderer3D* rend3D);
	void initRailing();
	void buildTransform();

private:

};