#include "Environment.h"

Environment::Environment(gef::Vector4 position, gef::Vector4 scale, gef::Vector4 rotation)
{
	m_position = position;
	m_scale = scale;
	m_rotation = rotation;

	//buildTransform();
}

Environment::~Environment()
{

}

void Environment::update(float dt)
{
	buildTransform();
}

void Environment::render(gef::Renderer3D* rend3D)
{
	rend3D->DrawMesh(*this);
}

void Environment::initGround(b2World* world, float xMeshSize, float yMeshSize)
{
	/*gef::Material* sandMat = new gef::Material();
	gef::Colour* sand = new gef::Colour(0.761f, 0.698f, 0.502f, 1);
	sandMat->set_colour(sand->GetABGR());

	gef::Material* matArr[6];

	for (int i = 0; i < 6; ++i)
	{
		matArr[i] = nullptr;
		matArr[i] = sandMat;
	}

	set_mesh(pb->CreateBoxMesh(m_scale * 0.5f, gef::Vector4(0, 0, 0), matArr));*/
	currentType = ObjectType::ENVIRONMENT;

	// Set up box2d static body for env.
	envBodyDef.type = b2_staticBody;
	envBodyDef.position.Set(m_position.x(), m_position.y());
	envBodyDef.userData.pointer = reinterpret_cast<uintptr_t>(this);

	// Create and attach the env body def.
	envBody = world->CreateBody(&envBodyDef);

	// Set up the env physics body shape.
	// *SOMEHOW NEED TO SET THIS TO THE SCALE OF THE MESH OF THIS OBJECT i.e. THE MESH OF THE LOADED MODEL*
	envPolygonShape.SetAsBox(xMeshSize * 0.5f, yMeshSize * 0.5f);

	// create the fixture
	envFixDef.shape = &envPolygonShape;
	envFixDef.friction = 10.0f;

	// create the fixture on the rigid body
	envBody->CreateFixture(&envFixDef);
}

void Environment::buildTransform()
{
	UpdateFromSimulation(envBody);

	/*transform_.SetTranslation(gef::Vector4(envBody->GetPosition().x, envBody->GetPosition().y, 0));
	set_transform(transform_);*/
}
