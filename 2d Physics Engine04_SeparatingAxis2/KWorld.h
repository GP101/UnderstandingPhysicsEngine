/*
	Windows porting in 2021: jintaeks@dongseo.ac.kr
*/

#ifndef _KWORLD_H_
#define _KWORLD_H_

#include "KMath.h"
#include "KManifold.h"
#include "KPhysicsEngine.h"

struct KWorld
{
public:
	static KWorld& Singleton();

	static const float		gravityScale;
	static const KVector2	gravity;
	static const float		dt;
	static bool				frameStepping;
	static bool				canStep;
	static const bool		enableFriction = true; // 20210428_jintaeks
	static const bool		drawPenetration = true;

public:
	/*constructor*/			KWorld(float dt);
	void					Step();
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
	float m_dt;
	std::vector<std::shared_ptr<KRigidbody>>	m_bodies;
	std::vector<KManifold>	m_contacts;
};

#define _KWorld		KWorld::Singleton()

#endif // _KWORLD_H_
