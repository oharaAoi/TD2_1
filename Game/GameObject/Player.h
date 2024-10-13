#pragma once
#include "Engine/GameObject/BaseGameObject.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/Input/Input.h"
#include "Engine/Lib/GameTimer.h"
#include "Engine/Collider/Collider.h"
#include "Easing.h"

class Player :
	public BaseGameObject,
	public Collider {
public:

	Player();
	~Player();

	void Init() override;
	void Update() override;
	void Draw() const override;

	void Move();
	void MoveLimit();
	void LookAtDirection(const float& angle);

	void AdaptAdjustmentItem();

	void OnCollision([[maybe_unused]] Collider* other) override {};
	const Vector3 GetWorldTranslation(const Vector3& offset = { 0.0f, 0.0f, 0.0f }) const;

public:

	void SetHitWaterSurface(const bool& ishit) { hitWaterSurface_ = ishit; }
	const Vector3 GetMoveVelocity() const { return velocity_ * moveSpeed_; }
	WorldTransform* GetAboveSurfaceTransform(){ return aboveWaterSurfacePos.get(); }
	float GetSwimmingDepth(){ return swimmigDepth_; }
	bool GetIsFlying(){ return isFlying_; }
	const Vector3& GetVelocity()const{ return velocity_; }

#ifdef _DEBUG
	void Debug_Gui();
#endif // _DEBUG

private:

	AdjustmentItem* adjustmentItem_;

	Quaternion restPoseRotation_;

	// パラメータ
	Vector3 velocity_;
	float moveSpeed_;
	float lookAtT_;

	// プレイヤーが泳ぐ際のの角度決定に関する変数
	float pressTime_;
	float currentAngle_;
	const float kMaxAngle_ = 3.14f * 0.28f;

	// フラグ
	bool hitWaterSurface_;
	bool isMove_ = true;
	bool isFlying_;

	// プレイヤーの上部の水面の座標
	std::unique_ptr<WorldTransform> aboveWaterSurfacePos;
	// プレイヤーがどれだけ潜っているか
	float swimmigDepth_;

public:// アクセッサ

	void SetIsMove(bool flag){ isMove_ = flag; }
};

