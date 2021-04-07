#include "Ruby.h"

/////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR / DESTRUCTOR
Ruby::Ruby(gef::Vector4 position, gef::Vector4 scale, gef::Vector4 rotation, bool isHUD)
{
	m_position = position;
	m_scale = scale;
	m_rotation = rotation;
	m_startPos = position;
	isHUDRuby = isHUD;
	rubyBody = nullptr;
	speed = 0;
	force = -10.0f;
	//buildTransform();
}

Ruby::~Ruby()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS
void Ruby::update(float dt)
{
	if (!isHUDRuby)
	{
		move(dt);
	}
	
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
	currentType = ObjectType::COLLECTABLE;

	// Set up box2d static body for env.
	rubyBodyDef.type = b2_dynamicBody;
	rubyBodyDef.position.Set(m_position.x(), m_position.y());
	rubyBodyDef.userData.pointer = reinterpret_cast<uintptr_t>(this);

	// Create and attach the env body def.
	rubyBody = world->CreateBody(&rubyBodyDef);

	// Set up the env physics body shape.
	rubyPolygonShape.SetAsBox(xMeshSize, yMeshSize);

	// create the fixture
	rubyFixDef.shape = &rubyPolygonShape;
	rubyFixDef.density = 80;
	rubyFixDef.friction = 100.0f;

	// create the fixture on the rigid body
	rubyBody->CreateFixture(&rubyFixDef);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Ruby::buildTransform()
{
	// setup object rotation
	rotX.RotationX(m_rotation.x());
	rotY.RotationY(m_rotation.y());
	rotZ.RotationZ(m_rotation.z());

	if (isHUDRuby)
	{
		m_position = gef::Vector4(m_position.x(), m_position.y(), m_position.z());
	}
	else
	{
		m_position = gef::Vector4(rubyBody->GetPosition().x, rubyBody->GetPosition().y, 0);
	}	

	// Set the scale
	sca.Scale(m_scale);

	// build object transformation matrix
	transform_ = rotX * rotY * rotZ * sca;
	transform_.SetTranslation(m_position);
	set_transform(transform_);

	//UpdateFromSimulation(rubyBody);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Ruby::move(float dt)
{
	force = 3000;
	speed = 2.0f;

	// If the ruby position ever becomes less than it's start pos,
	// and it should as it falls under gravity.
	// Then give it a little push up and then let gravity do its job again.
	// This give the ruby a nice bounce effect.
	if (rubyBody->GetPosition().y < (m_startPos.y()))
	{
		rubyBody->ApplyForceToCenter(b2Vec2(0, force * speed * dt), true);
	}	
}

/////////////////////////////////////////////////////////////////////////////////////////////////

// GETTERS / SETTERS
void Ruby::setPosition(gef::Vector4 position)
{
	m_position = position;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Ruby::setRotation(gef::Vector4 newRot)
{
	m_rotation = newRot;
}

/////////////////////////////////////////////////////////////////////////////////////////////////