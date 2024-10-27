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
	/// 桁数を割り出す
	/// </summary>
	/// <returns></returns>
	int DegitCount(float value);

	int IntegerCount(int value, int n);

	Vector2 CalculationSpriteLT(int value);

	void AdaptAdjustmentItem();

	void SpriteMove();

	void AddTime(float time);

	void AddClockMove();

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

	std::unique_ptr<Sprite> clock_;
	std::vector<std::unique_ptr<Sprite>> limitTimeUI_;
	Vector2 originPos_;

	Vector2 numberSpriteSize_ = { 48.0f, 72.0f };
	Vector2 numberSpriteScale_;
	float numberInterval_;

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
};

