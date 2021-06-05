#include "KCircleShape.h"


KCircleShape::KCircleShape(float r)
{
	radius = r;
}

void KCircleShape::Initialize()
{
	ComputeMass(1.0f);
}

void KCircleShape::ComputeMass(float density)
{
	body->m_mass = PI * radius * radius * density;
	body->m_invMass = (body->m_mass) ? 1.0f / body->m_mass : 0.0f;
	body->m_I = body->m_mass * radius * radius;
	body->m_invI = (body->m_I) ? 1.0f / body->m_I : 0.0f;
}

void KCircleShape::SetRotation(float radians)
{
}

KShape::Type KCircleShape::GetType() const
{
	return Type::eCircle;
}
