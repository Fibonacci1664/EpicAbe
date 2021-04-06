#include "Ruby.h"

/////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR / DESTRUCTOR
Ruby::Ruby(gef::Vector4 position, gef::Vector4 scale, gef::Vector4 rotation)
{
	m_position = position;
	m_scale = scale;
	m_rotation = rotation;
	envBody = nullptr;
	//buildTransform();
}

Ruby::~Ruby()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS
void Ruby::update(float dt)
{
	buildTransform();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Ruby::render(gef::Renderer3D* rend3D)
{
	rend3D->DrawMesh(*this);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Ruby::initRuby(b2World* world, float xMeshSize, float yMeshSize)
{
	currentType = ObjectType::ENVIRONMENT;

	// Set up box2d static body for env.
	envBodyDef.type = b2_staticBody;
	envBodyDef.position.Set(m_position.x() + xMeshSize * 0.2f, m_position.y());
	envBodyDef.userData.pointer = reinterpret_cast<uintptr_t>(this);

	// Create and attach the env body def.
	envBody = world->CreateBody(&envBodyDef);

	// Set up the env physics body shape.
	envPolygonShape.SetAsBox(xMeshSize * 0.2f, yMeshSize * 0.1f);

	// create the fixture
	envFixDef.shape = &envPolygonShape;
	envFixDef.friction = 10.0f;

	// create the fixture on the rigid body
	envBody->CreateFixture(&envFixDef);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Ruby::buildTransform()
{
	// setup object rotation
	rotX.RotationX(m_rotation.x());
	rotY.RotationY(m_rotation.y());
	rotZ.RotationZ(m_rotation.z());

	// setup the object translation
	m_position = gef::Vector4(m_position.x(), m_position.y(), m_position.z());

	// Set the scale
	sca.Scale(m_scale);

	// build object transformation matrix
	transform_ = rotX * rotY * rotZ * sca;
	transform_.SetTranslation(m_position);
	set_transform(transform_);

	//UpdateFromSimulation(envBody);
}

/////////////////////////////////////////////////////////////////////////////////////////////////