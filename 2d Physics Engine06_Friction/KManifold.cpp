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

void KManifold::Solve()
{
	g_collLookup[rigidbodyA->shape->GetType()][rigidbodyB->shape->GetType()](*this, rigidbodyA->shape, rigidbodyB->shape);
}

void KManifold::Initialize()
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

void KManifold::ApplyImpulse()
{
	// Early out and positional correct if both objects have infinite mass
	if (IsEqual(rigidbodyA->m_invMass + rigidbodyB->m_invMass, 0))
	{
		InfiniteMassCorrection();
		return;
	}

	for (uint32 i = 0; i < contact_count; ++i)
	{
		// Calculate radii from COM to contact
		KVector2 ra = contacts[i] - rigidbodyA->position;
		KVector2 rb = contacts[i] - rigidbodyB->position;

		// Relative velocity
		KVector2 rv = rigidbodyB->velocity + KVector2::Cross(rigidbodyB->angularVelocity, rb) -
			rigidbodyA->velocity - KVector2::Cross(rigidbodyA->angularVelocity, ra);

		// Relative velocity along the normal
		float contactVel = KVector2::Dot(rv, normal);

		// Do not resolve if velocities are separating
		if (contactVel > 0)
			return;

		float raCrossN = KVector2::Cross(ra, normal);
		float rbCrossN = KVector2::Cross(rb, normal);
		float invMassSum = rigidbodyA->m_invMass + rigidbodyB->m_invMass
			+ Square(raCrossN) * rigidbodyA->m_invI + Square(rbCrossN) * rigidbodyB->m_invI;

		// Calculate impulse scalar
		float j = -(1.0f + restitution) * contactVel;
		j /= invMassSum;
		j /= (float)contact_count;

		// Apply impulse
		KVector2 impulse = normal * j;
		rigidbodyA->ApplyImpulse(-impulse, ra);
		rigidbodyB->ApplyImpulse(impulse, rb);

		// Friction impulse
		if(KWorld::enableFriction == true )
		{
			rv = rigidbodyB->velocity + KVector2::Cross(rigidbodyB->angularVelocity, rb)
				- rigidbodyA->velocity - KVector2::Cross(rigidbodyA->angularVelocity, ra);

			KVector2 t = rv - (normal * KVector2::Dot(rv, normal));
			t.Normalize();

			// j tangent magnitude
			float jt = KVector2::Dot(rv, t);
			jt /= invMassSum;
			jt /= (float)contact_count;

			// Don't apply tiny friction impulses
			if (IsEqual(jt, 0.0f))
				return;

			// Couloumb's law
			KVector2 tangentImpulse;
			if (std::abs(jt) < j * sf)
				tangentImpulse = t * -jt;
			else
				tangentImpulse = t * -j * df;

			// Apply friction impulse
			rigidbodyA->ApplyImpulse(-tangentImpulse, ra);
			rigidbodyB->ApplyImpulse(tangentImpulse, rb);
		}/**/
	}
}

void KManifold::PositionalCorrection()
{
	const float k_slop = 0.05f; // Penetration allowance
	const float percent = 0.4f; // Penetration percentage to correct
	KVector2 correction = (__max(penetration - k_slop, 0.0f) / (rigidbodyA->m_invMass + rigidbodyB->m_invMass)) * normal * percent;
	rigidbodyA->position -= correction * rigidbodyA->m_invMass;
	rigidbodyB->position += correction * rigidbodyB->m_invMass;
}

void KManifold::InfiniteMassCorrection()
{
	rigidbodyA->velocity.Set(0, 0);
	rigidbodyB->velocity.Set(0, 0);
}
