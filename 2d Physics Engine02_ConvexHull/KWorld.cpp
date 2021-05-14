/*
	Windows porting in 2021: jintaeks@dongseo.ac.kr

	Copyright (c) 2013 Randy Gaul http://RandyGaul.net

	This software is provided 'as-is', without any express or implied
	warranty. In no event will the authors be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:
	  1. The origin of this software must not be misrepresented; you must not
		 claim that you wrote the original software. If you use this software
		 in a product, an acknowledgment in the product documentation would be
		 appreciated but is not required.
	  2. Altered source versions must be plainly marked as such, and must not be
		 misrepresented as being the original software.
	  3. This notice may not be removed or altered from any source distribution.
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

std::shared_ptr<KRigidbody> KWorld::Add(std::shared_ptr<KShape> shape, int32 x, int32 y)
{
	assert(shape);
	std::shared_ptr<KRigidbody> b;
	b.reset(new KRigidbody(shape, x, y));

	shape->body = b;
	shape->Initialize();

	m_bodies.push_back(b);
	return b;
}

void KWorld::Clear()
{
	m_bodies.clear();
}

std::shared_ptr<KShape> KWorld::CreateCircle(uint32 radius, int32 x, int32 y)
{
	std::shared_ptr<KCircleShape> c;
	c.reset(new KCircleShape((float)radius));
	std::shared_ptr<KRigidbody> body = Add(c, x, y);
	body->BodyToShape();

	return c;
}

std::shared_ptr<KShape> KWorld::CreatePolygon(KVector2* vertices, uint32 numVertices, int32 x, int32 y)
{
	std::shared_ptr<KPolygonShape> polygon;
	polygon.reset(new KPolygonShape());
	{
		polygon->Set(vertices, numVertices);
		std::shared_ptr<KRigidbody> body = Add(polygon, 0, 0);
		body->SetRotation(0);
		body->position = KVector2((float)x, (float)y);
		body->BodyToShape();
	}
	return polygon;
}

std::shared_ptr<KShape> KWorld::CreateBox(int32 width, int32 height, int32 x, int32 y)
{
	std::shared_ptr<KPolygonShape> polygon;
	polygon.reset(new KPolygonShape());
	{
		polygon->SetBox((float)width, (float)height);
		std::shared_ptr<KRigidbody> body = Add(polygon, x, y);
		body->SetRotation(0);
		body->BodyToShape();
	}
	return polygon;
}
