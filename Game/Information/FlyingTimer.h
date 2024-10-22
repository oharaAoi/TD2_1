#pragma once
#include <vector>
#include "Engine.h"

/// <summary>
/// 飛んでいる時間を計測するクラス
/// </summary>
class FlyingTimer {
public:

	FlyingTimer();
	~FlyingTimer();

	void Init();
	void Update(float posX);
	void Draw() const;

	/// <summary>
	/// 計測を行う
	/// </summary>
	void Measurement(float posX);

	/// <summary>
	/// 計測終了
	/// </summary>
	void Finish(float posX);

#ifdef _DEBUG
	void Debug_Gui();
#endif

	const float GetCurrentLength() const { return currentFlyingLength_; }
	const float GetMaxLnegth() const { return maxFlyingLength_; }

	static float flyingScor;

private:
	bool isMeasurement_; // 計測をしているか
	// 現在の飛んでいる量
	float currentFlyingLength_;
	// 最大の飛んだ距離
	float maxFlyingLength_;

	// 開始位置
	float flyingStartPosX_;
	float flyingFinalPosX_;
};

