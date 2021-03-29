#pragma once
#include <graphics/renderer_3d.h>
#include <box2d/box2d.h>

#include "../object/game_object.h"

class Environment : public GameObject
{
public:
	Environment(gef::Vector4 position, gef::Vector4 scale, gef::Vector4 rotation);
	~Environment();

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