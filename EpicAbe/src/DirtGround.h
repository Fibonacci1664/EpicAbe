#pragma once
#include <graphics/renderer_3d.h>
#include "game_object.h"

class DirtGround : public GameObject
{
public:
	DirtGround(gef::Vector4 position, gef::Vector4 scale, gef::Vector4 rotation);
	~DirtGround();

	void render(gef::Renderer3D* rend3D);
	void initDirt();
	void buildTransform();

private:


};

