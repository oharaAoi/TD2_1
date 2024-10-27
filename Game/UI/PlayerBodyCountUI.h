#pragma once
#include <memory>
#include <string>
#include <list>
#include "Engine/Engine.h"
#include "Engine/2d/Sprite.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/Math/Easing.h"
#include "Engine/Math/Vector4.h"

/// <summary>
/// Playerの体の数を数える
/// </summary>
class PlayerBodyCountUI {
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

	PlayerBodyCountUI();
	~PlayerBodyCountUI();

	void Init();
	void Update(int playerBodyCount);
	void Draw() const;

	void Move();

	void EmiteEffect();

#ifdef _DEBUG
	void Debug_Gui();
#endif

	void SetIsUiMove(bool isMove) { isUiMove_ = isMove; }

private:

	AdjustmentItem* adjust_;
	std::unique_ptr<Sprite> maxBody_UI_ = nullptr;

	std::unique_ptr<Sprite> gaugeBack_UI_ = nullptr;
	std::unique_ptr<Sprite> gaugeFront_UI_ = nullptr;
	
	Vector2 uiPos_;

	// Parameter
	float time_;
	float moveTime_ = 2.0f;

	Vector2 fadeInStartPos_;
	Vector2 fadeOutPos_;

	Vector2 backPos_UI_;
	Vector2 frontPos_UI_;

	Vector2 frontSize_UI_;

	// フラグ
	bool isUiMove_;
	bool isFadeIn_;

	int preCount_;

	// -------------------------------------------------
	// ↓ UIEffect用
	// -------------------------------------------------

	std::list<EffectData> effectList_;
	float effectMoveTime_;

};

