#pragma once
#include "../object/game_object.h"

/////////////////////////////////////////////////////////////////////////////////////////////////

#include <graphics/renderer_3d.h>
#include <box2d/box2d.h>

/////////////////////////////////////////////////////////////////////////////////////////////////

class Ground : public GameObject
{
public:
	Ground(gef::Vector4 position, gef::Vector4 scale, gef::Vector4 rotation);
	~Ground();

	void update(float dt);
	void render(gef::Renderer3D* rend3D);
	void initGround(b2World* world, float xMeshSize, float yMeshSize);
	void buildTransform();

private:

	// ENV PHYSICS BODY STUFF
	b2BodyDef envBodyDef;
	b2Body* envBody;
	b2PolygonShape envPolygonShape;
	b2FixtureDef envFixDef;
};

/////////////////////////////////////////////////////////////////////////////////////////////////