#pragma once
#include "Engine/GameObject/BaseGameObject.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/Input/Input.h"
#include "Engine/Lib/GameTimer.h"
#include "Engine/Collider/Collider.h"
#include "Easing.h"
#include "Game/Attachment/PlayerAnimator.h"
#include "Engine/Audio/AudioPlayer.h"

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

	void OnCollision([[maybe_unused]] Collider* other) override;
	const Vector3 GetWorldTranslation() const;

public:

	void SetHitWaterSurface(const bool& ishit) { hitWaterSurface_ = ishit; }
	const Vector3 GetMoveVelocity() const { return velocity_ * moveSpeed_; }
	const float GetMoveSpeed()const{ return moveSpeed_; }
	WorldTransform* GetAboveSurfaceTransform(){ return aboveWaterSurfacePos.get(); }
	float GetSwimmingDepth(){ return swimmigDepth_; }
	bool GetIsFlying(){ return isFlying_; }
	const Vector3& GetVelocity()const{ return velocity_; }

	const uint32_t GetCoinNum() const { return getCoinNum_; }

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

	std::unique_ptr<PlayerAnimator> animetor_;

	// sound
	std::unique_ptr<AudioPlayer> hitSe_;
	std::unique_ptr<AudioPlayer> coinGetSe_;

	uint32_t getCoinNum_ = 0;

public:// アクセッサ

	void SetIsMove(bool flag){ isMove_ = flag; }
};

