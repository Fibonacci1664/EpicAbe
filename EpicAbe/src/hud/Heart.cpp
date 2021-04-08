#include "Heart.h"

/////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR / DESTRUCTOR
Heart::Heart(gef::Vector4 position, gef::Vector4 scale, gef::Vector4 rotation, bool isHUD)
{
	m_position = position;
	m_scale = scale;
	m_rotation = rotation;
	m_startPos = position;
	isHUDHeart = isHUD;
	heartBody = nullptr;
	theWorld = nullptr;
	speed = 0;
	force = -10.0f;
	alive = true;
	bodyDestroyed = false;
}

Heart::~Heart()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS
void Heart::update(float dt)
{
	if (!isHUDHeart)
	{
		move(dt);
	}

	if (!alive)
	{
		destroyBody();
	}

	buildTransform();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Heart::render(gef::Renderer3D* rend3D)
{
	rend3D->DrawMesh(*this);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Heart::initHeart(b2World* world, float xMeshSize, float yMeshSize)
{
	theWorld = world;

	currentType = ObjectType::COLLECTABLE;

	// Set up box2d static body for env.
	heartBodyDef.type = b2_dynamicBody;
	heartBodyDef.position.Set(m_position.x(), m_position.y());
	heartBodyDef.userData.pointer = reinterpret_cast<uintptr_t>(this);

	// Create and attach the env body def.
	heartBody = world->CreateBody(&heartBodyDef);

	// Set up the env physics body shape.
	heartPolygonShape.SetAsBox(xMeshSize, yMeshSize);

	// create the fixture
	heartFixDef.shape = &heartPolygonShape;
	heartFixDef.density = 80;
	heartFixDef.friction = 100.0f;

	// create the fixture on the rigid body
	heartBody->CreateFixture(&heartFixDef);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Heart::buildTransform()
{
	// setup object rotation
	rotX.RotationX(m_rotation.x());
	rotY.RotationY(m_rotation.y());
	rotZ.RotationZ(m_rotation.z());

	if (isHUDHeart)
	{
		m_position = gef::Vector4(m_position.x(), m_position.y(), m_position.z());
	}
	else
	{
		m_position = gef::Vector4(heartBody->GetPosition().x, heartBody->GetPosition().y, 0);
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

void Heart::move(float dt)
{
	force = 3000;
	speed = 2.0f;

	// If the heart position ever becomes less than it's start pos,
	// and it should as it falls under gravity.
	// Then give it a little push up and then let gravity do its job again.
	// This give the ruby a nice bounce effect.
	if (heartBody->GetPosition().y < (m_startPos.y()))
	{
		heartBody->ApplyForceToCenter(b2Vec2(0, force * speed * dt), true);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Heart::setIsAlive(bool isAlive)
{
	alive = isAlive;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

bool Heart::getIsAlive()
{
	return alive;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

bool Heart::getIsBodyDestroyed()
{
	return bodyDestroyed;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Heart::destroyBody()
{
	theWorld->DestroyBody(heartBody);
	bodyDestroyed = true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

// GETTERS / SETTERS
void Heart::setPosition(gef::Vector4 position)
{
	m_position = position;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Heart::setRotation(gef::Vector4 newRot)
{
	m_rotation = newRot;
}

/////////////////////////////////////////////////////////////////////////////////////////////////