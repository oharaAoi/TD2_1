#pragma once
#include <cstdint>
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Vector4.h"

/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider {
public:

	static bool isColliderBoxDraw_;

public:

	Collider();
	virtual ~Collider();

	/// <summary>
	/// 衝突時に呼ばれる関数
	/// </summary>
	virtual void OnCollision([[maybe_unused]] Collider* other) {};

public:

	// ------------ 位置(world) ------------ // 
	const virtual Vector3 GetWorldTranslation(const Vector3& offset = { 0.0f, 0.0f, 0.0f }) const = 0;

	// ------------ 半径 ------------ // 
	float GetRadius() const { return radius_; }
	void SetRadius(const float& radius) { radius_ = radius; }

	// ------------ Hitしているか ------------ // 
	void SetIsHitting(const bool& hit) { isHitting_ = hit; }
	const bool GetIsHitting() const { return isHitting_; }

protected:

	// 半径
	float radius_ = 1.0f;
	// 種別ID
	uint32_t typeID_ = 0u;

	bool isHitting_;
	Vector4 color_;

};

