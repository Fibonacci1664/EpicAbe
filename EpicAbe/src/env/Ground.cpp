#include "Ground.h"

Ground::Ground(gef::Vector4 position, gef::Vector4 scale, gef::Vector4 rotation)
{
	m_position = position;
	m_scale = scale;
	m_rotation = rotation;

	//buildTransform();
}

Ground::~Ground()
{

}

void Ground::update(float dt)
{
	buildTransform();
}

void Ground::render(gef::Renderer3D* rend3D)
{
	rend3D->DrawMesh(*this);
}

void Ground::initGround(b2World* world, float xMeshSize, float yMeshSize)
{
	currentType = ObjectType::ENVIRONMENT;

	// Set up box2d static body for env.
	envBodyDef.type = b2_staticBody;
	envBodyDef.position.Set(m_position.x(), m_position.y());
	envBodyDef.userData.pointer = reinterpret_cast<uintptr_t>(this);

	// Create and attach the env body def.
	envBody = world->CreateBody(&envBodyDef);

	// Set up the env physics body shape.
	// *SOMEHOW NEED TO SET THIS TO THE SCALE OF THE MESH OF THIS OBJECT i.e. THE MESH OF THE LOADED MODEL*
	envPolygonShape.SetAsBox(xMeshSize * 0.32f, yMeshSize * 0.32f);

	// create the fixture
	envFixDef.shape = &envPolygonShape;
	envFixDef.friction = 10.0f;

	// create the fixture on the rigid body
	envBody->CreateFixture(&envFixDef);
}

void Ground::buildTransform()
{
	UpdateFromSimulation(envBody);
}
