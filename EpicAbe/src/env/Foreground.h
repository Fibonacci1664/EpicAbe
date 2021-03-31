#pragma once
#include <graphics/renderer_3d.h>
#include <box2d/box2d.h>
#include "../object/game_object.h"

class Foreground : public GameObject
{
public:
	Foreground(gef::Vector4 position, gef::Vector4 scale, gef::Vector4 rotation);
	~Foreground();

	void render(gef::Renderer3D* rend3D);
	void initForeground();
	void buildTransform();

private:

};

