#pragma once
#include "../object/game_object.h"

/////////////////////////////////////////////////////////////////////////////////////////////////

#include <graphics/renderer_3d.h>
#include <box2d/box2d.h>

/////////////////////////////////////////////////////////////////////////////////////////////////

class Ruby : public GameObject
{
public:
	Ruby(gef::Vector4 position, gef::Vector4 scale, gef::Vector4 rotation, bool isHUD);
	~Ruby();

	void update(float dt);
	void render(gef::Renderer3D* rend3D);
	void initRuby(b2World* world, float xMeshSize, float yMeshSize);
	void buildTransform();
	void move(float dt);

	void setPosition(gef::Vector4 position);
	void setRotation(gef::Vector4 newRot);

private:

	float speed;
	bool isHUDRuby;
	float force;

	// ENV PHYSICS BODY STUFF
	b2BodyDef rubyBodyDef;
	b2Body* rubyBody;
	b2PolygonShape rubyPolygonShape;
	b2FixtureDef rubyFixDef;
};

/////////////////////////////////////////////////////////////////////////////////////////////////