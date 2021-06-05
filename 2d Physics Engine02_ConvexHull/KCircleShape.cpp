#include "KCircleShape.h"


KCircleShape::KCircleShape(float r)
{
	radius = r;
}

void KCircleShape::Initialize()
{
}

void KCircleShape::SetRotation(float radians)
{
}



KShape::Type KCircleShape::GetType() const
{
	return Type::eCircle;
}
