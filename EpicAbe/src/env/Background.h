/*
 * This class controls:
 *		- Rendering the mesh of the model.
 *		- Building the transform of the object.
 *
 * Original @author D. Green.
 *
 * � D. Green. 2021.
 */

/////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../object/game_object.h"

/////////////////////////////////////////////////////////////////////////////////////////////////

#include <graphics/renderer_3d.h>
#include <box2d/box2d.h>

/////////////////////////////////////////////////////////////////////////////////////////////////

class Background : public GameObject
{
public:
	Background(gef::Vector4 position, gef::Vector4 scale, gef::Vector4 rotation);
	~Background();

	void render(gef::Renderer3D* rend3D);
	void initBackground();
	void buildTransform();

private:

};

/////////////////////////////////////////////////////////////////////////////////////////////////