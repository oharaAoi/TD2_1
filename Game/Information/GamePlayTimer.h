#pragma once
#include <vector>
#include "Engine.h"
#include "Engine/Lib/GameTimer.h"
#include "Engine/Audio/AudioPlayer.h"
#include "Engine/Utilities/AdjustmentItem.h"

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

#ifdef _DEBUG
	void Debug_Gui();
#endif

	const bool GetIsFinish() { return isFinish_; }

private:
	AdjustmentItem* adjustItem_;
	std::string groupName_;

	// 終了したか
	bool isFinish_;
	bool isOverTime_;
	bool isPreFlying_;
	// 時間系
	float gameTimer_;
	float gameTimeLimit_;

	std::unique_ptr<Sprite> clock_;
	std::vector<std::unique_ptr<Sprite>> limitTimeUI_;
	Vector2 originPos_;

	Vector2 numberSpriteSize_ = { 42.0f, 72.0f };
	Vector2 numberSpriteScale_;
	float numberInterval_;

	std::unique_ptr<AudioPlayer> timeleft10s_;
	std::unique_ptr<AudioPlayer> timeleft60s_;
};

