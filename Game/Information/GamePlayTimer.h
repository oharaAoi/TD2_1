#pragma once
#include <vector>
#include "Engine.h"
#include "Engine/Lib/GameTimer.h"
#include "Engine/Audio/AudioPlayer.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/2d/Sprite.h"
#include "Engine/Math/Easing.h"

/// <summary>
/// ゲームの進行している時間を計測するクラス
/// </summary>
class GamePlayTimer {
public:

	GamePlayTimer();
	~GamePlayTimer();

	void Init(float limit);
	void Update(bool isPlayerFlying);
	void Draw() const;

	/// <summary>
	/// 計測を行う
	/// </summary>
	void Measurement();

	/// <summary>
	/// TimeUpの文字のscaleを大きくする
	/// </summary>
	void BigNumberScalUp();

	/// <summary>
	/// 調整項目の適応
	/// </summary>
	void AdaptAdjustmentItem();

	/// <summary>
	/// 残り時間通知Textureの移動
	/// </summary>
	void SpriteMove();

	/// <summary>
	/// 時間の追加
	/// </summary>
	/// <param name="time"></param>
	void AddTime(float time);

	/// <summary>
	/// 時間追加の際の時計の移動
	/// </summary>
	void AddClockMove();

	/// <summary>
	/// 桁数を割り出す
	/// </summary>
	/// <returns></returns>
	/*int DegitCount(float value);

	int IntegerCount(int value, int n);

	Vector2 CalculationSpriteLT(int value);*/

#ifdef _DEBUG
	void Debug_Gui();
#endif

	static const bool GetIsFinish() { return isFinish_; }
	const float GetTimeLinit()const{ return gameTimer_; }
	const float GetOutgameTime()const{ return outgameTime_; }

private:
	AdjustmentItem* adjustItem_;
	std::string groupName_;

	// 終了したか
	static bool isFinish_;
	bool isOverTime_;
	bool isPreFlying_;
	// 時間系
	float gameTimer_;
	float gameTimeLimit_;
	float outgameTime_;

	/*std::unique_ptr<Sprite> clock_;*/
	std::unique_ptr<Sprite> timeGauge_;
	std::unique_ptr<Sprite> timeGaugeOutSide_;
	std::unique_ptr<Sprite> bigNumberUI_;
	Vector2 originPos_;

	Vector2 timeGaugeBarOffset_;

	// タイムを追加した際のUI
	std::unique_ptr<Sprite> addTimeSprite_;
	bool isAddTime_;

	float addClockMoveCount_;
	float addClockMoveTime_;
	bool isAppearance_;

	Vector2 addTimeEndPos_;
	Vector4 addClockColor_;
	float alpa_;

	// 残り時間のUI
	std::unique_ptr<AudioPlayer> timeleft10s_;
	std::unique_ptr<AudioPlayer> timeleft60s_;

	// 終了前のUI
	std::unique_ptr<Sprite> timeleftUI_;

	bool isMove_;
	bool isFadeIn_;

	float time_;
	float moveTime_;

	Vector2 startPos_;
	Vector2 endPos_;
	Vector2 uiPos_;

	// ------------------- scaleUp用の変数 ------------------- //

	Vector2 bigNumberScale_;
	float scaleUpTime_;
	float scaleUpTimeLimit_ = 0.4f;
	
	float bigNumberAlpha_;

	float slopeAngle_ = 10.0f;
};


/*Vector2 numberSpriteSize_ = { 48.0f, 72.0f };
	Vector2 numberSpriteScale_;
	float numberInterval_;*/