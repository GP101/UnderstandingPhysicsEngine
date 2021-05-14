#include "KPhysicsEngine.h"

/*static*/ KWorld& KWorld::Singleton()
{
	static KWorld instance;
	return instance;
}

// constructor
KWorld::KWorld() 
{
}
