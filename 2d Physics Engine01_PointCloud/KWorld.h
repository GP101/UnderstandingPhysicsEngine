#ifndef _KWORLD_H_
#define _KWORLD_H_

#include "KMath.h"
#include "KPhysicsEngine.h"

struct KWorld
{
public:
	static KWorld& Singleton();

public:
	/*constructor*/			KWorld();
};

#define _KWorld		KWorld::Singleton()

#endif // _KWORLD_H_
