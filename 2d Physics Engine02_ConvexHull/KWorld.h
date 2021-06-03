/*
	Windows porting in 2021: jintaeks@dongseo.ac.kr
*/

#ifndef _KWORLD_H_
#define _KWORLD_H_

#include "KMath.h"
#include "KPhysicsEngine.h"

struct KWorld
{
public:
	static KWorld& Singleton();

	static const float		gravityScale;
	static const KVector2	gravity;

public:
	/*constructor*/			KWorld();
	std::shared_ptr<KRigidbody>
							CreateRigidbody(std::shared_ptr<KShape> shape, float x, float y);
	std::shared_ptr<KRigidbody>
							Add(std::shared_ptr<KShape> shape, float x, float y);
	void					Clear();
	// Factory members
	std::shared_ptr<KShape> CreateCircle(float radius, float x, float y, bool isStatic = false);
	std::shared_ptr<KShape> CreatePolygon(KVector2* vertices, uint32 numVertices, float x, float y, bool isStatic = false);
	std::shared_ptr<KShape> CreateBox(float width, float height, float x, float y, bool isStatic = false);

public:
	std::vector<std::shared_ptr<KRigidbody>>	m_bodies;
};

#define _KWorld		KWorld::Singleton()

#endif // _KWORLD_H_
