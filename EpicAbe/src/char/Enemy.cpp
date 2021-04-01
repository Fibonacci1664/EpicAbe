#include "Enemy.h"

#include "../char/Player.h"

Enemy::Enemy(gef::Vector4 position, gef::Vector4 scale, gef::Vector4 rotation)
{
	m_position = position;
	m_scale = scale;
	m_rotation = rotation;
	speed = 0.9f;
	m_startPos = position;			// Start position is saved in case we ever need to reset back to start pos.
}

Enemy::~Enemy()
{

}

void Enemy::handleInput(float dt)
{

}

void Enemy::update(float dt)
{
	buildTransform();
}

void Enemy::render(gef::Renderer3D* rend3D)
{
	rend3D->DrawMesh(*this);
}

void Enemy::initEnemy(b2World* world, float xMeshSize, float yMeshSize, float randFriction)
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
	enemyFixtureDef.friction = randFriction;

	// create the fixture on the rigid body
	enemyBody->CreateFixture(&enemyFixtureDef);
}

void Enemy::followPlayer(Player* player, float dt)
{
	/*gef::Vector4 playerPos = gef::Vector4(	player->getPlayerBody()->GetPosition().x,
											player->getPlayerBody()->GetPosition().y, 1.0f);*/



	//m_position.Lerp(gef::Vector4(enemyBody->GetPosition().x, enemyBody->GetPosition().y, 1.0f), playerPos, 1.0f * dt);

	float playerDirectionX = player->getPlayerBody()->GetPosition().x - enemyBody->GetPosition().x;

	// This says WHERE we want to LERP from and to, and over what time period.
	//m_position.Lerp(m_position, *player->getPosition(), dt);

	// This says how much force should be applied in the direction of the LERP.
	enemyBody->ApplyLinearImpulseToCenter(b2Vec2(playerDirectionX * speed * dt, 0), true);
	//m_rotation.set_y(-player->getLeftStickAngle() * dt);
}

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

gef::Vector4* Enemy::getPosition()
{
	return &m_position;
}

void Enemy::setPosition(gef::Vector4* newPosition)
{
	m_position = *newPosition;
}

void Enemy::setRotation(gef::Vector4 newRot)
{
	m_rotation.set_x(newRot.x());
	m_rotation.set_y(newRot.y());
	m_rotation.set_z(newRot.z());
}

void Enemy::setSpeed(float newSpeed)
{
	speed *= newSpeed;
}
