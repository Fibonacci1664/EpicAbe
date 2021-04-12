/*
 * This class controls all aspects of the enemy including:
 *		- Initilaising Physics body.
 *		- Logic for moving the model.
 *		- Rendering the mesh.
 *		- Updating the transform.
 *
 * Original @author D. Green.
 *
 * © D. Green. 2021.
 */

 /////////////////////////////////////////////////////////////////////////////////////////////////

#include "Enemy.h"
#include "../char/Player.h"

/////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR / DESTRUCTOR
Enemy::Enemy(gef::Vector4 position, gef::Vector4 scale, gef::Vector4 rotation)
{
	enemyBody = nullptr;
	m_position = position;
	m_scale = scale;
	m_rotation = rotation;
	maxSpeed = 4;
	speed = 4;
	m_startPos = position;			// Start position is saved in case we ever need to reset back to start pos.
}

Enemy::~Enemy()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS
void Enemy::handleInput(float dt)
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Enemy::update(float dt)
{
	move(dt);
	buildTransform();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Enemy::render(gef::Renderer3D* rend3D)
{
	rend3D->DrawMesh(*this);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Enemy::initEnemy(b2World* world, float xMeshSize, float yMeshSize)
{
	currentType = ObjectType::ENEMY;

	// Set up box2d static body for env.
	enemyBodyDef.type = b2_dynamicBody;
	enemyBodyDef.position.Set(m_position.x(), m_position.y());
	enemyBodyDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
	enemyBodyDef.fixedRotation = true;

	// Create and attach the env body def.
	enemyBody = world->CreateBody(&enemyBodyDef);

	// Set up the env physics body shape.
	enemyPolygonShape.SetAsBox(xMeshSize * 1.75f * m_scale.x(), yMeshSize * 0.14f * m_scale.y());

	// create the fixture
	enemyFixtureDef.shape = &enemyPolygonShape;
	enemyFixtureDef.density = 1.0f;
	enemyFixtureDef.friction = 0;

	// create the fixture on the rigid body
	enemyBody->CreateFixture(&enemyFixtureDef);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Enemy::move(float dt)
{
	float force = 2.0f;

	// If the enemy position ever becomes greater or lesser than 1 unit away from its starting position
	// Then negate the force. This causes the enemy to move back and forth in an area.
	if (enemyBody->GetPosition().x > (m_startPos.x() + 1) || enemyBody->GetPosition().x < (m_startPos.x() - 1))
	{
		force = -force;

		// After we have started applying an opposite force and once vel == 0, flip the model
		// ready to moce the opposite direction.
		if (enemyBody->GetLinearVelocity().x < 0.05)
		{
			setRotation(gef::Vector4(0, 0, 3.1415 * 1.5f));
		}
	}
	else
	{
		if (enemyBody->GetLinearVelocity().x > -0.05)
		{
			setRotation(gef::Vector4(0, 0, 3.1415 * 0.5f));
		}
	}

	// Where we are applying the force and along what axis.
	enemyBody->ApplyForceToCenter(b2Vec2(force * speed * dt, 0), true);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Enemy::buildTransform()
{
	//setup object rotation
	rotZ.RotationZ(m_rotation.z());
	rotY.RotationY(m_rotation.y());

	// setup the object translation
	m_position = gef::Vector4(enemyBody->GetPosition().x, enemyBody->GetPosition().y, 0);

	// Set the scale
	sca.Scale(m_scale);

	// build object transformation matrix
	transform_ = rotY * rotZ * sca;
	transform_.SetTranslation(m_position);
	set_transform(transform_);

	//UpdateFromSimulation(enemyBody);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

// GETTTERS / SETTERS
gef::Vector4* Enemy::getPosition()
{
	return &m_position;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Enemy::setPosition(gef::Vector4* newPosition)
{
	m_position = *newPosition;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Enemy::setRotation(gef::Vector4 newRot)
{
	m_rotation.set_x(newRot.x());
	m_rotation.set_y(newRot.y());
	m_rotation.set_z(newRot.z());
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Enemy::setSpeed(float newSpeed)
{
	speed *= newSpeed;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Enemy::setStartLocation(float startLoc)
{
	m_startPos.set_x(startLoc);
}

/////////////////////////////////////////////////////////////////////////////////////////////////