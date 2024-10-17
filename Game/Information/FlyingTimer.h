#pragma once

/// <summary>
/// 飛んでいる時間を計測するクラス
/// </summary>
class FlyingTimer {
public:

	FlyingTimer();
	~FlyingTimer();

	void Init();
	void Update(float posX);

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

private:
	bool isMeasurement_; // 計測をしているか
	// 現在の飛んでいる量
	float currentFlyingLength_;
	// 最大の飛んだ距離
	float maxFlyingLength_;

	/// <summary>
	/// 開始位置
	/// </summary>
	float flyingStartPosX_;

	float flyingFinalPosX_;
};

