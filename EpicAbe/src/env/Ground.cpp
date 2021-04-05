#include "Ground.h"

/////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR / DESTRUCTOR
Ground::Ground(gef::Vector4 position, gef::Vector4 scale, gef::Vector4 rotation)
{
	m_position = position;
	m_scale = scale;
	m_rotation = rotation;
	envBody = nullptr;
	//buildTransform();
}

Ground::~Ground()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS
void Ground::update(float dt)
{
	buildTransform();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Ground::render(gef::Renderer3D* rend3D)
{
	rend3D->DrawMesh(*this);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

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
	envPolygonShape.SetAsBox(xMeshSize * 0.5f, yMeshSize * 0.1f);

	// create the fixture
	envFixDef.shape = &envPolygonShape;
	envFixDef.friction = 10.0f;

	// create the fixture on the rigid body
	envBody->CreateFixture(&envFixDef);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Ground::buildTransform()
{
	// setup object rotation
	//rotZ.RotationZ(m_rotation.z());
	//rotY.RotationY(m_rotation.y());

	//// setup the object translation
	//m_position = gef::Vector4(m_position.x(), m_position.y(), m_position.z());

	//// Set the scale
	//sca.Scale(m_scale);

	//// build object transformation matrix
	//transform_ = rotY * rotZ * sca;
	//transform_.SetTranslation(m_position);
	//set_transform(transform_);

	UpdateFromSimulation(envBody);
}

/////////////////////////////////////////////////////////////////////////////////////////////////