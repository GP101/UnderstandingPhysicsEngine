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

#include "KManifold.h"
#include "Collision.h"
#include "KPhysicsEngine.h"


KManifold::KManifold(std::shared_ptr<KRigidbody> a, std::shared_ptr<KRigidbody> b)
	: rigidbodyA(a), rigidbodyB(b)
{
	penetration = 0.0f;
	contact_count = 0;
	restitution = 0.0f;
	df = 0.0f;
	sf = 0.0f;
}

void KManifold::Solve(void)
{
	g_collLookup[rigidbodyA->shape->GetType()][rigidbodyB->shape->GetType()](*this, rigidbodyA->shape, rigidbodyB->shape);
}

void KManifold::Initialize(void)
{
	// Calculate average restitution
	restitution = __min(rigidbodyA->restitution, rigidbodyB->restitution);

	// Calculate static and dynamic friction
	sf = std::sqrt(rigidbodyA->staticFriction * rigidbodyB->staticFriction);
	df = std::sqrt(rigidbodyA->dynamicFriction * rigidbodyB->dynamicFriction);

	for (uint32 i = 0; i < contact_count; ++i)
	{
		// Calculate radii from COM to contact
		KVector2 ra = contacts[i] - rigidbodyA->position;
		KVector2 rb = contacts[i] - rigidbodyB->position;

		KVector2 rv = rigidbodyB->velocity + KVector2::Cross(rigidbodyB->angularVelocity, rb) -
			rigidbodyA->velocity - KVector2::Cross(rigidbodyA->angularVelocity, ra);

		// Determine if we should perform a resting collision or not
		// The idea is if the only thing moving this object is gravity,
		// then the collision should be performed without any restitution
		if (rv.LengthSquared() < (KWorld::dt * KWorld::gravity).LengthSquared() + EPSILON)
			restitution = 0.0f;
	}
}