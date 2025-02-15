#pragma once
#include <vector>
#include "Engine.h"
#include "Engine/Lib/GameTimer.h"
#include <Vector2.h>
#include "Engine/Utilities/AdjustmentItem.h"

enum SpeedRaitoState {
	Raito_0,
	Raito_30,
	Raito_70,
	Raito_100,
};

/// <summary>
/// Playerの時間を表示するクラス
/// </summary>
class PlayerSpeedCounter {
public:

	PlayerSpeedCounter();
	~PlayerSpeedCounter();

	void Init();
	void Update(float speed, float raito, float alpha, bool isPlayerFlying);
	void Draw() const;

	// ------------------- Speedの割合に関する処理 ------------------- //

	void SpeedRaitoUpdate(float speed);

	void SpeedAnnounceMove();

	// ------------------- SpeedMaxに関する処理 ------------------- //
	void SpeedMaxUpdate();
	void SpeedMaxMove();

	void SetMaxSpeed(float maxSpeed) { maxSpeed_ = maxSpeed; }


#ifdef _DEBUG
	void Debug_Gui();
#endif // _DEBUG

private:

	/// <summary>
	/// 桁数を割り出す
	/// </summary>
	/// <returns></returns>
	float DegitCount(float value);

	/// <summary>
	/// 実数値の計算
	/// </summary>
	/// <param name="value"></param>
	/// <param name="n"></param>
	/// <returns></returns>
	float IntegerCount(float value, int n);

	/// <summary>
	/// Spriteの左上座標を決める
	/// </summary>
	/// <param name="value"></param>
	/// <returns></returns>
	Vector2 CalculationSpriteLT(float value);

	/// <summary>
	/// 新しい桁を追加する
	/// </summary>
	/// <param name="array"></param>
	//void CreateNewDigite(std::vector<std::unique_ptr<Sprite>>& array, float value, uint32_t digite, const Vector2& origin);

private:

	void AdaptAdjustmentItem();

private:

	AdjustmentItem* adjustmentItem_;
	const std::string groupName_ = "PlayerSpeedCounterUI";

	Vector2 numberSpriteSize_ = { 48.0f, 72.0f };
	Vector2 numberOriginPos_;

	Vector2 timeDrawScale_;

	std::unique_ptr<Sprite> backSprite_;
	std::unique_ptr<Sprite> taniSprite_;
	std::unique_ptr<Sprite> needleSprite_;
	std::vector<std::unique_ptr<Sprite>> UI_speed_;

	uint32_t currentDigit_;
	uint32_t maxDigit_;

	float needleAngleMax_ = 2.12f;
	float easeT=0.1f;
	float easeRatio;

	float preSpeed_;

	float maxSpeed_;

	// -------------------------------------------------
	// ↓ speedMax
	// -------------------------------------------------
	std::unique_ptr<Sprite> speedMaxUI_ = nullptr;
	// Parameter
	float time_;
	float moveTime_ = 1.8f;

	Vector2 fadeInStartPos_;
	Vector2 fadeOutPos_;
	Vector2 speedMaxPos_;

	// フラグ
	bool isUiMove_;
	bool isFadeIn_;
	bool isFinish_;

	// -------------------------------------------------
	// ↓ Speedのアナウンス
	// -------------------------------------------------

	SpeedRaitoState speedRaitoState_ = Raito_0;
	bool isAnnounce_ = false;

	std::unique_ptr<Sprite> speedSprite_;
	std::unique_ptr<Sprite> percentSprite_;
	std::unique_ptr<Sprite> speedAnnounceNumber_[2];

	Vector2 announcePos_;
	Vector2 speedLocalPos_ = {-100.0f, 0.0f};
	Vector2 percentLocalPos_ = { 125.0f, 0.0f };
	Vector2 numberLocalPos_;
	Vector2 numberSpriteDivision_ = {64.0f, 0.0f};

	// Parameter
	float announceTime_;
	float announceMoveTime_ = 1.8f;

	Vector2 announceFadeInStartPos_;
	Vector2 announceFadeOutPos_;
	Vector2 announceSpeedMaxPos_;

	// フラグ
	bool isAnnounceUiMove_;
	bool isAnnounceFadeIn_;
	bool isAnnounceFinish_;

};

