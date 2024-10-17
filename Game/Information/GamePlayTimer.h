#pragma once
#include "Engine/Lib/GameTimer.h"

/// <summary>
/// ゲームの進行している時間を計測するクラス
/// </summary>
class GamePlayTimer {
public:

	GamePlayTimer();
	~GamePlayTimer();

	void Init(float limit);
	void Update();

	/// <summary>
	/// 計測を行う
	/// </summary>
	void Measurement();

#ifdef _DEBUG
	void Debug_Gui();
#endif

	const bool GetIsFinish() { return isFinish_; }

private:

	bool isFinish_;

	float gameTimer_;
	float gameTimeLimit_;

};

