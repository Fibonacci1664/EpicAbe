#pragma once
#include "../object/game_object.h"

/////////////////////////////////////////////////////////////////////////////////////////////////

#include <graphics/renderer_3d.h>
#include <box2d/box2d.h>


/////////////////////////////////////////////////////////////////////////////////////////////////

class LevelEndDoor : public GameObject
{
public:
	LevelEndDoor(gef::Vector4 position, gef::Vector4 scale, gef::Vector4 rotation);
	~LevelEndDoor();

	void render(gef::Renderer3D* rend3D);
	void initLevelEndDoor();
	void buildTransform();

private:

};

/////////////////////////////////////////////////////////////////////////////////////////////////