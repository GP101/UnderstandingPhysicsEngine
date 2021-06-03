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

#ifndef MANIFOLD_H
#define MANIFOLD_H

#include <memory>
#include "KRigidbody.h"

struct KRigidbody;
struct KManifold// : public std::enable_shared_from_this<KManifold>
{
	KManifold(std::shared_ptr<KRigidbody> rigidA, std::shared_ptr<KRigidbody> rigidB);
	void Solve();                 // Generate contact information
	void Initialize();            // Precalculations for impulse solving
	void ApplyImpulse();          // Solve impulse and apply
	void PositionalCorrection();  // Naive correction of positional penetration
	void InfiniteMassCorrection();

	std::shared_ptr<KRigidbody> rigidbodyA;
	std::shared_ptr<KRigidbody> rigidbodyB;

	float penetration;     // Depth of penetration from collision
	KVector2 normal;          // From A to B
	KVector2 contacts[2];     // Points of contact during collision
	uint32 contact_count;	// Number of contacts that occurred during collision
	float restitution;		// Mixed restitution
	float df;              // Mixed dynamic friction
	float sf;              // Mixed static friction
};

#endif // MANIFOLD_H