#include "Enemy.h"

#include "../char/Player.h"

Enemy::Enemy(gef::Vector4 position, gef::Vector4 scale, gef::Vector4 rotation)
{
	m_position = position;
	m_scale = scale;
	m_rotation = rotation;

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
	UpdateFromSimulation(enemyBody);
}

void Enemy::move(char direction, float scale, float dt)
{

}

void Enemy::checkBounds()
{

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
