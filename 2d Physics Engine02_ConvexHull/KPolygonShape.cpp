#include "KPolygonShape.h"
#include "KMath.h"

extern HDC     g_hdc;


void KPolygonShape::Initialize()
{
}

bool KPolygonShape::IsValid() const
{
	return body != nullptr;
}

void KPolygonShape::SetRotation(float radians)
{
	rotation.Set(radians);
}

void KPolygonShape::Draw() const
{
	COLORREF color;
	color = RGB(r*255.f, g * 255.f, b*255.f);
	std::vector<KVector2> points;
	for (uint32 i = 0; i < m_vertices.size(); ++i)
	{
		KVector2 v = body->position + rotation * m_vertices[i];
		points.push_back(KVector2(v.x, v.y));
	}
	KVectorUtil::DrawLine(g_hdc, points, 2, 0, color, 1); // 1: line loop
}

KShape::Type KPolygonShape::GetType() const
{
	return ePoly;
}

// Half width and half height
void KPolygonShape::SetBox(float hw, float hh)
{
	//m_vertexCount = 4;
	m_vertices.resize(4);
	m_normals.resize(4);
	m_vertices[0].Set(-hw, -hh);
	m_vertices[1].Set(hw, -hh);
	m_vertices[2].Set(hw, hh);
	m_vertices[3].Set(-hw, hh);
	m_normals[0].Set(0.0f, -1.0f);
	m_normals[1].Set(1.0f, 0.0f);
	m_normals[2].Set(0.0f, 1.0f);
	m_normals[3].Set(-1.0f, 0.0f);
}

void KPolygonShape::Set(KVector2* vertices, uint32 count)
{
	// No hulls with less than 3 vertices (ensure actual polygon)
	assert(count > 2 && count <= MaxPolyVertexCount);
	count = __min((int32)count, MaxPolyVertexCount);

	// Find the right most point on the hull
	int32 rightMost = 0;
	float highestXCoord = vertices[0].x;
	for (uint32 i = 1; i < count; ++i)
	{
		float x = vertices[i].x;
		if (x > highestXCoord)
		{
			highestXCoord = x;
			rightMost = i;
		}

		// If matching x then take farthest negative y
		else if (x == highestXCoord)
			if (vertices[i].y < vertices[rightMost].y)
				rightMost = i;
	}

	int32 hull[MaxPolyVertexCount];
	int32 outCount = 0;
	int32 indexHull = rightMost;
	uint32 vertexCount = 0;

	for (;;)
	{
		hull[outCount] = indexHull;

		// Search for next index that wraps around the hull
		// by computing cross products to find the most counter-clockwise
		// vertex in the set, given the previos hull index
		int32 nextHullIndex = 0;
		for (int32 i = 1; i < (int32)count; ++i)
		{
			// Skip if same coordinate as we need three unique
			// points in the set to perform a cross product
			if (nextHullIndex == indexHull)
			{
				nextHullIndex = i;
				continue;
			}

			// Cross every set of three unique vertices
			// Record each counter clockwise third vertex and add
			// to the output hull
			// See : http://www.oocities.org/pcgpe/math2d.html
			KVector2 e1 = vertices[nextHullIndex] - vertices[hull[outCount]];
			KVector2 e2 = vertices[i] - vertices[hull[outCount]];
			float c = KVector2::Cross(e1, e2);
			if (c < 0.0f)
				nextHullIndex = i;

			// Cross product is zero then e vectors are on same line
			// therefor want to record vertex farthest along that line
			if (c == 0.0f && e2.LengthSquared() > e1.LengthSquared())
				nextHullIndex = i;
		}

		++outCount;
		indexHull = nextHullIndex;

		// Conclude algorithm upon wrap-around
		if (nextHullIndex == rightMost)
		{
			vertexCount = outCount;
			break;
		}
	}

	m_vertices.resize(vertexCount);
	m_normals.resize(vertexCount);
	// Copy vertices into shape's vertices
	for (uint32 i = 0; i < vertexCount; ++i)
		m_vertices[i] = vertices[hull[i]];

	// Compute face normals
	for (uint32 i1 = 0; i1 < vertexCount; ++i1)
	{
		uint32 i2 = i1 + 1 < vertexCount ? i1 + 1 : 0;
		KVector2 face = m_vertices[i2] - m_vertices[i1];

		// Ensure no zero-length edges, because that's bad
		assert(face.LengthSquared() > EPSILON * EPSILON);

		// Calculate normal with 2D cross product between vector and scalar
		m_normals[i1] = KVector2(face.y, -face.x);
		m_normals[i1].Normalize();
	}
}
