#include "Dunes.h"

/////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR / DESTRUCTOR
Dunes::Dunes(gef::Vector4 position, gef::Vector4 scale, gef::Vector4 rotation)
{
	m_position = position;
	m_scale = scale;
	m_rotation = rotation;

	buildTransform();
}

Dunes::~Dunes()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS
void Dunes::render(gef::Renderer3D* rend3D)
{
	rend3D->DrawMesh(*this);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Dunes::initDunes()
{
	currentType = ObjectType::ENVIRONMENT;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Dunes::buildTransform()
{
	// setup object rotation
	rotZ.RotationZ(m_rotation.z());
	rotY.RotationY(m_rotation.y());

	// setup the object translation
	m_position = gef::Vector4(m_position.x(), m_position.y(), m_position.z());

	// Set the scale
	sca.Scale(m_scale);

	// build object transformation matrix
	transform_ = rotY * rotZ * sca;
	transform_.SetTranslation(m_position);
	set_transform(transform_);
}

/////////////////////////////////////////////////////////////////////////////////////////////////