#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

/////////////////////////////////////////////////////////////////////////////////////////////////

#include <graphics/mesh_instance.h>
#include <box2d/Box2D.h>
#include <maths/transform.h>

/////////////////////////////////////////////////////////////////////////////////////////////////

// FORWARD DECS
class b2Body;

enum class ObjectType
{
	UNDEFINED, PLAYER, ENEMY, ENVIRONMENT, COLLECTABLE
};

/////////////////////////////////////////////////////////////////////////////////////////////////

class GameObject : public gef::MeshInstance
{
public:
	GameObject();
	~GameObject();

	void UpdateFromSimulation(const b2Body* body);

	void setType(ObjectType type);
	ObjectType* getType();

protected:

	ObjectType currentType;

	gef::Matrix44 sca;
	gef::Matrix44 rotX;
	gef::Matrix44 rotY;
	gef::Matrix44 rotZ;
	gef::Matrix44 tra;

	gef::Vector4 m_scale;
	gef::Vector4 m_rotation;
	gef::Vector4 m_position;

	gef::Vector4 m_startPos;

private:

};

#endif // _GAME_OBJECT_H

/////////////////////////////////////////////////////////////////////////////////////////////////