#pragma once
#include <vector>
#include "Engine.h"
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
	void Draw();

	/// <summary>
	/// 計測を行う
	/// </summary>
	void Measurement();

#ifdef _DEBUG
	void Debug_Gui();
#endif

	const bool GetIsFinish() { return isFinish_; }

private:
	// 終了したか
	bool isFinish_;
	// 時間系
	float gameTimer_;
	float gameTimeLimit_;

	std::vector<Sprite> limitTimeUI_;

};

