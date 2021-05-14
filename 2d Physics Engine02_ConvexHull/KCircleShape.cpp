#include "KCircleShape.h"

extern HDC     g_hdc;

KCircleShape::KCircleShape(float r)
{
	radius = r;
}

void KCircleShape::Initialize(void)
{
}

void KCircleShape::SetRotation(float radians)
{
}

void KCircleShape::Draw() const
{
	const uint32 k_segments = 20;

	std::vector<KVector2> points;
	COLORREF color = RGB(r*255.f, g * 255.f, b*255.f);
	KVector2 v0;
	KVector2 v1;
	float theta = body->rotation;
	float inc = PI * 2.0f / (float)k_segments;
	for (uint32 i = 0; i < k_segments; ++i)
	{
		theta += inc;
		KVector2 p(std::cos(theta), std::sin(theta));
		p *= radius;
		p += body->position;
		points.push_back(KVector2(p.x, p.y));
	}
	KVectorUtil::DrawLine(g_hdc, points, 2, 0, color, 1);

	// Render line within circle so orientation is visible
	KVector2 r(0, 1.0f);
	float c = std::cos(body->rotation);
	float s = std::sin(body->rotation);
	r.Set(r.x * c - r.y * s, r.x * s + r.y * c);
	r *= radius;
	r = r + body->position;
	v0 = KVector2(body->position.x, body->position.y);
	v1 = KVector2(r.x, r.y);
	KVectorUtil::DrawArrow(g_hdc, v0, v1, 0.5f, 2, 0, color);
}

KShape::Type KCircleShape::GetType() const
{
	return eCircle;
}
