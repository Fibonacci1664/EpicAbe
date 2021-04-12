/*
 * This is the parent class for all game objects.
 *
 * Original @author Abertay University
 * 
 * Updated by @author D. Green.
 *
 * © D. Green. 2021.
 */

/////////////////////////////////////////////////////////////////////////////////////////////////

#include "game_object.h"

/////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR / DESTRUCTOR
GameObject::GameObject()
{
	currentType = ObjectType::UNDEFINED;

	sca.SetIdentity();
	rotX.SetIdentity();
	rotY.SetIdentity();
	rotZ.SetIdentity();
	tra.SetIdentity();

	transform_.SetIdentity();

	m_scale = gef::Vector4(1.0f, 1.0f, 1.0f);
	m_rotation = gef::Vector4::kZero;
	m_position = gef::Vector4::kZero;

	m_startPos = gef::Vector4::kZero;
}

GameObject::~GameObject()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////

//
// UpdateFromSimulation
// 
// Update the transform of this object from a physics rigid body
//
void GameObject::UpdateFromSimulation(const b2Body* body)
{
	if (body)
	{
		// setup object rotation
		rotZ.RotationZ(body->GetAngle());
		rotY.RotationY(m_rotation.y());

		// setup the object translation
		m_position = gef::Vector4(body->GetPosition().x, body->GetPosition().y, 0);

		// Set the scale
		sca.Scale(m_scale);

		// build object transformation matrix
		transform_ = rotY * rotZ * sca;
		transform_.SetTranslation(m_position);
		set_transform(transform_);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

// GETTERS / SETTERS
void GameObject::setType(ObjectType type)
{
	currentType = type;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

ObjectType* GameObject::getType()
{
	return &currentType;
}

/////////////////////////////////////////////////////////////////////////////////////////////////