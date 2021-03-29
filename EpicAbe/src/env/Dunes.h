#pragma once
#include "../object/game_object.h"

#include <graphics/renderer_3d.h>

class Dunes : public GameObject
{
public:
	Dunes(gef::Vector4 position, gef::Vector4 scale, gef::Vector4 rotation);
	~Dunes();

	void render(gef::Renderer3D* rend3D);
	void initDunes();
	void buildTransform();

private:


};
