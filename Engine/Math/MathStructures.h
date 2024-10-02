#pragma once
#include "MyMath.h"

/// <summary>
/// 軸平行境界箱
/// </summary>
struct AABB {
	Vector3 min;
	Vector3 max;

	AABB(const Vector3& center, const float& radius);
};
