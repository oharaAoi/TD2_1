#pragma once
#include "Engine/GameObject/BaseGameObject.h"
#include "Game/GameObject/PlayerBody.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/Input/Input.h"
#include "Engine/Lib/GameTimer.h"
#include "Engine/Collider/Collider.h"
#include "Easing.h"
#include "Game/Attachment/PlayerAnimator.h"
#include "Engine/Audio/AudioPlayer.h"
#include "Game/Information/FlyingTimer.h"

class Player :
	public BaseGameObject,
	public Collider{
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

	void UpdateBody();
	void AddBody(BaseGameObject* pTarget);
	void EraseBody();

public:

	void SetHitWaterSurface(const bool& ishit){ hitWaterSurface_ = ishit; }
	const float GetMoveSpeed()const{ return std::clamp(baseSpeed_ + temporaryAcceleration_,kMinMoveSpeed_,kMaxMoveSpeed_); }
	const Vector3 GetMoveVelocity() const{ return velocity_ * GetMoveSpeed();}
	WorldTransform* GetAboveSurfaceTransform(){ return aboveWaterSurfacePos.get(); }
	float GetSwimmingDepth(){ return swimmigDepth_; }
	bool GetIsFlying(){ return isFlying_; }
	const Vector3& GetVelocity()const{ return velocity_; }
	const uint32_t GetCoinNum() const{ return getCoinNum_; }
	const bool GetIsMove() const {return isMove_;}
	const bool GetIsSplash()const{ return isSplash_; }

#ifdef _DEBUG
	void Debug_Gui();
#endif // _DEBUG

	const float GetFlyingTime() const { return timer_.GetCurrentLength(); }
	const float GetMaxFlyingTime() const { return timer_.GetMaxLnegth(); }

private:

	AdjustmentItem* adjustmentItem_;
	std::unique_ptr<PlayerAnimator> animetor_;

	// 付属モデル---------------------------------------

	std::list<std::unique_ptr<PlayerBody>>followModels_;
	const int kMinBodyCount_ = 2;
	const int kMaxBodyCount_ = 8;
	int bodyCount_ = kMinBodyCount_;

	// パラメータ---------------------------------------

	Quaternion restPoseRotation_;
	Quaternion slerpRotation_;

	float lookAtT_;

	// 移動パラメーター
	Vector3 velocity_;
	float baseSpeed_;// 加速しない状態での速度
	float temporaryAcceleration_;// 一時的な加速度
	const float defaultSpeed = 35.0f;
	const float kMinMoveSpeed_ = 25.0f;
	const float kMaxMoveSpeed_ = 150.0f;

	// アイテム取得時などに追加する速度
	float increaseVelocity_ = 40.0f;
	float decreaseVelocity_ = -20.0f;

	// プレイヤーが泳ぐ際のの角度決定に関する変数
	float pressTime_;
	float currentAngle_;
	const float kMaxAngle_ = 3.14f * 0.38f;

	// 食べてチャージして溜める
	float chargePower_;
	const float kBodyLaunchSpeed_ = 10.0f;

	// 着水して水に潜った際の猶予時間
	const float kDiveTime_ = 1.0f;
	float diveTime_ = kDiveTime_;

	// 落下時の重力
	float gravity_ = -2.0f;
	float dropSpeed_;

	// フラグ-------------------------------------------

	bool hitWaterSurface_;
	bool isMove_ = true;
	bool isFlying_;
	bool preFlying_; // 前フレームで飛んでいたか
	bool isFalling_;// 下降中かどうか
	bool isDiving_;	// 飛行後終了して水に入った瞬間を得るフラグ
	bool isCloseWing_;// 飛行中に翼を閉じているかどうか
	bool isSplash_;// 水しぶき発生フラグ
	bool isFacedBird_;// 鳥に正面衝突したかどうか

	// データ格納変数　----------------------------------

	std::unique_ptr<WorldTransform> aboveWaterSurfacePos;// プレイヤーの上部の水面の座標
	float swimmigDepth_;	// プレイヤーがどれだけ潜っているか
	uint32_t getCoinNum_ = 0;// コインを何枚取得したか
	Vector3 prePos_;
	float divingSpeed_;// 着水時の下へのスピード

	// 飛距離計測用　----------------------------------
	FlyingTimer timer_;

	// sound-------------------------------------------

	std::unique_ptr<AudioPlayer> hitSe_;
	std::unique_ptr<AudioPlayer> coinGetSe_;


public:// アクセッサ

	void SetIsMove(bool flag){ isMove_ = flag; }
};

