#include "KPolygonShape.h"
#include "KMath.h"
#include <algorithm>
#include <stack>


void KPolygonShape::Initialize()
{
	ComputeMass(1.0f);
}

bool KPolygonShape::IsValid() const
{
	return body != nullptr;
}

void KPolygonShape::ComputeMass(float density)
{
	// Calculate centroid and moment of inertia
	KVector2 c(0.0f, 0.0f); // centroid
	float area = 0.0f;
	float I = 0.0f;
	const float k_inv3 = 1.0f / 3.0f;

	for (uint32 i1 = 0; i1 < m_vertices.size(); ++i1)
	{
		// Triangle vertices, third vertex implied as (0, 0)
		KVector2 p1(m_vertices[i1]);
		uint32 i2 = i1 + 1 < m_vertices.size() ? i1 + 1 : 0;
		KVector2 p2(m_vertices[i2]);

		float D = KVector2::Cross(p1, p2);
		float triangleArea = 0.5f * D;

		area += triangleArea;

		// Use area to weight the centroid average, not just vertex position
		c += triangleArea * k_inv3 * (p1 + p2);

		float intx2 = p1.x * p1.x + p2.x * p1.x + p2.x * p2.x;
		float inty2 = p1.y * p1.y + p2.y * p1.y + p2.y * p2.y;
		I += (0.25f * k_inv3 * D) * (intx2 + inty2);
	}

	c *= 1.0f / area;

	// Translate vertices to centroid (make the centroid (0, 0)
	// for the polygon in model space)
	// Not really necessary, but I like doing this anyway
	for (uint32 i = 0; i < m_vertices.size(); ++i)
		m_vertices[i] -= c;

	body->m_mass = density * area;
	body->m_invMass = (body->m_mass) ? 1.0f / body->m_mass : 0.0f;
	body->m_I = I * density;
	body->m_invI = body->m_I ? 1.0f / body->m_I : 0.0f;
}

void KPolygonShape::SetRotation(float radians)
{
	rotation.Set(radians);
}

KShape::Type KPolygonShape::GetType() const
{
	return Type::ePoly;
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
	FindConvexHull(vertices, count, m_vertices);

	const int vertexCount = m_vertices.size();
	m_normals.resize(vertexCount);

	// Compute face normals
	for (int i1 = 0; i1 < vertexCount; ++i1)
	{
		int i2 = i1 + 1 < vertexCount ? i1 + 1 : 0;
		KVector2 face = m_vertices[i2] - m_vertices[i1];

		// Ensure no zero-length edges, because that's bad
		assert(face.LengthSquared() > EPSILON * EPSILON);

		// Calculate normal with 2D cross product between vector and scalar
		m_normals[i1] = KVector2(face.y, -face.x);
		m_normals[i1].Normalize();
	}
}

static KVector2 p0;

void KPolygonShape::FindConvexHull(KVector2 points[], int n, std::vector<KVector2>& convexHullPoints)
{
	auto SecondTop = [](std::stack<KVector2>& stk)
	{
		KVector2 tempPoint = stk.top();
		stk.pop();
		KVector2 res = stk.top();    //get the second top element
		stk.push(tempPoint);      //push previous top again
		return res;
	};
	auto compare = [](const void* point1, const void* point2) ->int
	{
		KVector2* p1 = (KVector2*)point1;
		KVector2* p2 = (KVector2*)point2;
		int dir = KVector2::GetDirection(p0, *p1, *p2);
		if (dir == 0)
			return (KVector2::DistSquared(p0, *p2) >= KVector2::DistSquared(p0, *p1)) ? -1 : 1;
		return (dir == 2) ? -1 : 1;
	};

	float minY = points[0].y;
	int min = 0;
	for (int i = 1; i < n; i++) {
		float y = points[i].y;
		// find bottom left most point
		if ((y < minY) || (minY == y) && points[i].x < points[min].x) {
			minY = points[i].y;
			min = i;
		}
	}
	std::swap(points[0], points[min]); // swap min point to 0th location
	p0 = points[0];
	std::qsort(&points[1], n - 1, sizeof(KVector2), compare);
	int arrSize = 1; // used to locate items in modified array
	for (int i = 1; i < n; i++) {
		// when the angle of ith and (i+1)th elements are same, remove points
		while (i < n - 1 && KVector2::GetDirection(p0, points[i], points[i + 1]) == 0)
			i++;
		points[arrSize] = points[i];
		arrSize++;
	}
	if (arrSize < 3)
		return; // there must be at least 3 points, return empty list.

	// create a stack and add first three points in the stack
	std::stack<KVector2> stk;
	stk.push(points[0]);
	stk.push(points[1]);
	stk.push(points[2]);
	for (int i = 3; i < arrSize; i++) {    //for remaining vertices
		while (KVector2::GetDirection(SecondTop(stk), stk.top(), points[i]) != 2)
			stk.pop(); // when top, second top and ith point are not making left turn, remove point
		stk.push(points[i]);
	}
	const int numPoints = stk.size();
	int index = numPoints - 1;
	convexHullPoints.resize( numPoints );
	while (!stk.empty()) {
		convexHullPoints[index] = stk.top(); //add points from stack
		stk.pop();
		index -= 1;
	}
}

// The extreme point along a direction within a polygon
KVector2 KPolygonShape::GetSupportPoint(const KVector2& dir)
{
	float bestProjection = -FLT_MAX;
	KVector2 bestVertex;

	for (uint32 i = 0; i < m_vertices.size(); ++i)
	{
		KVector2 v = m_vertices[i];
		float projection = KVector2::Dot(v, dir);

		if (projection > bestProjection)
		{
			bestVertex = v;
			bestProjection = projection;
		}
	}

	return bestVertex;
}
