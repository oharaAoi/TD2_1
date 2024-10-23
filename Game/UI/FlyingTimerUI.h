#pragma once
#include <vector>
#include "Engine.h"

class FlyingTimerUI {
public:

	FlyingTimerUI();
	~FlyingTimerUI();

	void Init();
	void Update(float current, float max);
	void Draw(float alpha);

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
	void CreateNewDigite(std::vector<std::unique_ptr<Sprite>>& array, float value, uint32_t digite, const Vector2& origin);



#ifdef _DEBUG
	void Debug_Gui();
#endif // _DEBUG

private:
	std::vector<std::unique_ptr<Sprite>> UI_flyingLength_;
	std::vector<std::unique_ptr<Sprite>> UI_flyingMaxLength_;
	Vector2 rangeSize_ = { 42.0f, 72.0f };

	float currentLength_;
	float maxLength_;
	uint32_t currentDigit_;
	uint32_t maxDigit_;

	Vector2 currentPos_;
	Vector2 maxPos_;

};