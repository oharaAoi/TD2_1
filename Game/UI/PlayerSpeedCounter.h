#pragma once
#include <vector>
#include "Engine.h"
#include "Engine/Lib/GameTimer.h"
#include <Vector2.h>
#include "Engine/Utilities/AdjustmentItem.h"

/// <summary>
/// Playerの時間を表示するクラス
/// </summary>
class PlayerSpeedCounter {
public:

	PlayerSpeedCounter();
	~PlayerSpeedCounter();

	void Init();
	void Update(float speed);
	void Draw() const;

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

	void AdaptAdjustmentItem();

private:

	AdjustmentItem* adjustmentItem_;
	const std::string groupName_ = "PlayerSpeedCounterUI";

	Vector2 numberSpriteSize_ = { 64.0f, 64.0f };
	Vector2 numberOriginPos_;

	std::unique_ptr<Sprite> backSprite_;
	std::unique_ptr<Sprite> taniSprite_;
	std::vector<std::unique_ptr<Sprite>> UI_speed_;

	uint32_t currentDigit_;
	uint32_t maxDigit_;
};

