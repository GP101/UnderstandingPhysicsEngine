#include "KShapeUtil.h"
#include "KCircleShape.h"
#include "KPolygonShape.h"
#include <Windows.h>
#include <memory>

extern HDC g_hdc;

void KShapeUtil::Draw(std::shared_ptr<KShape> shape)
{
	if (shape->GetType() == KShape::eCircle)
		Draw(*std::dynamic_pointer_cast<KCircleShape>(shape));
	else if (shape->GetType() == KShape::ePoly)
		Draw(*std::dynamic_pointer_cast<KPolygonShape> (shape));
}

void KShapeUtil::Draw(KCircleShape& shape)
{
	COLORREF color = RGB(shape.r * 255.f, shape.g * 255.f, shape.b * 255.f);
	KVectorUtil::DrawCircle(g_hdc, shape.position, shape.radius, 20, 2, 0, color);

	// Render line within circle so orientation is visible
	KVector2 r(0, 1.0f);
	float c = std::cos(shape.body->rotation);
	float s = std::sin(shape.body->rotation);
	r.Set(r.x * c - r.y * s, r.x * s + r.y * c);
	r *= shape.radius;
	r = r + shape.position;
	KVector2 v0 = KVector2(shape.position.x, shape.position.y);
	KVector2 v1 = KVector2(r.x, r.y);
	KVectorUtil::DrawArrow(g_hdc, v0, v1, 0.5f, 2, 0, color);
}

void KShapeUtil::Draw(KPolygonShape& shape)
{
	COLORREF color;
	color = RGB(shape.r * 255.f, shape.g * 255.f, shape.b * 255.f);
	std::vector<KVector2> points;
	for (uint32 i = 0; i < shape.m_vertices.size(); ++i)
	{
		KVector2 v = shape.position + shape.rotation * shape.m_vertices[i];
		points.push_back(KVector2(v.x, v.y));
	}
	KVectorUtil::DrawPolygon(g_hdc, points, color);
}
