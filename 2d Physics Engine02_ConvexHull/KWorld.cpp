/*
	Windows porting in 2021: jintaeks@dongseo.ac.kr
*/

#include "KPhysicsEngine.h"

//const float			KWorld::gravityScale = 5.0f; // original 5.0f. 20210428_jintaeks
const float		KWorld::gravityScale = 0.0f; // test
const KVector2		KWorld::gravity(0, 9.8f * gravityScale);

/*static*/ KWorld& KWorld::Singleton()
{
	static KWorld instance;
	return instance;
}

// constructor
KWorld::KWorld() 
{
}

std::shared_ptr<KRigidbody> KWorld::CreateRigidbody(std::shared_ptr<KShape> shape, float x, float y)
{
	assert(shape);
	std::shared_ptr<KRigidbody> b;
	b.reset(new KRigidbody(shape, x, y));

	shape->body = b;
	shape->Initialize();
	return b;
}

std::shared_ptr<KRigidbody> KWorld::Add(std::shared_ptr<KShape> shape, float x, float y)
{
	std::shared_ptr<KRigidbody> b = CreateRigidbody(shape, x, y);
	m_bodies.push_back(b);
	return b;
}

void KWorld::Clear()
{
	m_bodies.clear();
}

std::shared_ptr<KShape> KWorld::CreateCircle(float radius, float x, float y, bool isStatic)
{
	std::shared_ptr<KCircleShape> c;
	c.reset(new KCircleShape((float)radius));
	std::shared_ptr<KRigidbody> body = Add(c, x, y);
	body->BodyToShape();

	return c;
}

std::shared_ptr<KShape> KWorld::CreatePolygon(KVector2* vertices, uint32 numVertices, float x, float y, bool isStatic)
{
	std::shared_ptr<KPolygonShape> polygon;
	polygon.reset(new KPolygonShape());
	{
		polygon->Set(vertices, numVertices);
		std::shared_ptr<KRigidbody> body = Add(polygon, 0, 0);
		body->SetRotation(0);
		body->position = KVector2(x, y);
		body->BodyToShape();
	}
	return polygon;
}

std::shared_ptr<KShape> KWorld::CreateBox(float width, float height, float x, float y, bool isStatic)
{
	std::shared_ptr<KPolygonShape> polygon;
	polygon.reset(new KPolygonShape());
	{
		polygon->SetBox(width, height);
		std::shared_ptr<KRigidbody> body = Add(polygon, x, y);
		body->SetRotation(0);
		body->BodyToShape();
	}
	return polygon;
}
