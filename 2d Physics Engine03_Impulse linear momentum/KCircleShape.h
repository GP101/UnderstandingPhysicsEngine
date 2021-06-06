#ifndef _KCIRCLESHAPE_H_
#define _KCIRCLESHAPE_H_

#include "KShape.h"
#include "KRigidbody.h"
#include "KMath.h"
#include "KVectorUtil.h"
#include <memory>

struct KCircleShape : public KShape
{
	KCircleShape(float r);
	void Initialize();
	void ComputeMass(float density);
	void SetRotation(float radians);
	KShape::Type GetType() const;
public:
	float radius = 0.0f;
};

#endif // _KCIRCLESHAPE_H_
