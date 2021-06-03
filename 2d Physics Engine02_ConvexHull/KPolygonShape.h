#ifndef _KPOLYGONSHAPE_H_
#define _KPOLYGONSHAPE_H_

#include "KShape.h"
#include "KRigidbody.h"
#include "KMath.h"
#include "KVectorUtil.h"

#define MaxPolyVertexCount 64


struct KPolygonShape : public KShape
{
	void Initialize();
	bool IsValid() const;
	void SetRotation(float radians);
	void Draw() const;
	KShape::Type GetType() const;
	// Half width and half height
	void SetBox(float hw, float hh);
	void Set(KVector2 *vertices, uint32 count);

	std::vector<KVector2> m_vertices;
	std::vector<KVector2> m_normals;
};

#endif // _KPOLYGONSHAPE_H_
