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
							Add(std::shared_ptr<KShape> shape, int32 x, int32 y);
	void					Clear();
	// Factory members
	std::shared_ptr<KShape> CreateCircle(uint32 radius, int32 x, int32 y);
	std::shared_ptr<KShape> CreatePolygon(KVector2* vertices, uint32 numVertices, int32 x, int32 y);
	std::shared_ptr<KShape> CreateBox(int32 width, int32 height, int32 x, int32 y);

public:
	std::vector<std::shared_ptr<KRigidbody>>	m_bodies;
};

#define _KWorld		KWorld::Singleton()

#endif // _KWORLD_H_
