#pragma once

#include "KCircleShape.h"
#include "KPolygonShape.h"

namespace KShapeUtil
{
	void Draw(std::shared_ptr<KShape> shape);
	void Draw(KCircleShape& shape);
	void Draw(KPolygonShape& shape);
}
