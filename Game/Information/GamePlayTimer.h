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

	std::vector<std::unique_ptr<Sprite>> limitTimeUI_;

};

