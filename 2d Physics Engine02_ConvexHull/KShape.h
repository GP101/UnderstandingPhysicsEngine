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

#ifndef SHAPE_H
#define SHAPE_H

#include "KRigidbody.h"
#include "KMath.h"
#include "KVectorUtil.h"
#include <memory>

struct KRigidbody;
struct KShape;
typedef std::shared_ptr<KShape>	KShapePtr;
struct KShape
{
	enum Type
	{
		eCircle,
		ePoly,
		eCount
	};

public:
	KShape() : body(nullptr)
	{
		r = Random(0.2f, 0.8f);
		g = Random(0.2f, 0.8f);
		b = Random(0.2f, 0.8f);
	}
	virtual void Initialize() = 0;
	//virtual void ComputeMass(float density) = 0;
	virtual void SetRotation(float radians) = 0;
	virtual void Draw() const = 0;
	virtual Type GetType() const = 0;
	void SetPosition(const KVector2& pos_) { position = pos_; }

public:
	std::shared_ptr<KRigidbody> body;
	// For circle shape
	float radius = 0.0f;
	// For Polygon shape
	KMatrix2 rotation; // Orientation matrix from model to world
	KVector2 position;
	// Store a color in RGB format
	float r, g, b;
};

#endif // SHAPE_H
