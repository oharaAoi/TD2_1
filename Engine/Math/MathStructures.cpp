#include "MathStructures.h"

AABB::AABB(const Vector3& center, const float& radius) {
	min = { center.x - radius, center.y - radius, center.z - radius };
	max = { center.x + radius, center.y + radius, center.z + radius };
}
