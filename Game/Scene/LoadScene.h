#pragma once
#include <memory>
#include "Engine/Engine.h"
#include "Engine/2d/Sprite.h"
#include "Game/UI/FadePanel.h"

/// <summary>
/// 鳥で飛べることを簡単に示唆するScene
/// </summary>
class LoadScene {
public:

	struct EffectData {
		std::unique_ptr<Sprite> effectSprite_;
		float moveTime_;
		Vector4 color_;
		float alpha_;

		EffectData() {
			effectSprite_ = Engine::CreateSprite("UIeffect.png");
			moveTime_ = 0.0f;
			color_ = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			alpha_ = 1.0f;
		}
	};

public:

	LoadScene();
	~LoadScene();

	void Init();
	void Update();
	void Draw() const;

	/// <summary>
	/// 調整項目の適応
	/// </summary>
	void AdaptAdjustmentItem();

	/// <summary>
	/// 鯉のアイコンを移動させる
	/// </summary>
	void CarpIconMove();

	/// <summary>
	/// Effectの追加
	/// </summary>
	void EmiteEffect();

#ifdef _DEBUG
	void Debug_Gui();
#endif
public:

	const bool GetIsLoadFinish() const { return isLoadFinish_; }

private:

	// ------------------- load ------------------- //
	std::string groupName_ = "loadScene";

	// ------------------- object ------------------- //
	// 背景
	std::unique_ptr<Sprite> background_ = nullptr;

	// 鯉
	std::unique_ptr<Sprite> carpIcon_ = nullptr;

	// 鳥
	std::unique_ptr<Sprite> birdIcon_[3];

	// パネル
	std::unique_ptr<FadePanel> fadePanel_ = nullptr;

	// 鳥を踏んだ時の演出
	std::list<EffectData> birdJumpEffectList_;

	// ------------------- parameter関連 ------------------- //
	Vector2 carpIconPos_;			// 鯉の座標
	Vector2 birdIconPos_[4];		// 鳥の座標
	Vector2 firstCarpIconPos_;		// 鯉の初期座標

	// ------------------- move関連 ------------------- //
	float moveTime_;
	float moveLimit_;

	float movePosY_;
	float moveTheta_;		// sin波
	float moveAmplitude_;	// 振幅

	uint32_t moveIndex_ = 0;

	bool isMove_ = false;			// 鯉が動いているか
	bool isLoadFinish_ = false;		// load画面を終了するか

	Vector2 preCarpIconPos_;		// 前フレームの鯉の座標

	// ------------------- Effect関連 ------------------- //
	float effectMoveTime_ = 0.0f;
};

