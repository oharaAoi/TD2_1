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

	struct BodyUIData {
		std::unique_ptr<Sprite> sprite_;
		float time_;

		BodyUIData() {
			sprite_ = Engine::CreateSprite("KoiGeuge_Torso.png");
			time_ = 0.0f;
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

	/// <summary>
	/// 胴体を追加
	/// </summary>
	void AddBody();

	/// <summary>
	/// 胴体を削除
	/// </summary>
	void ReleseBody();

	/// <summary>
	/// 胴体が落ちる演出
	/// </summary>
	void DropBody();

	/// <summary>
	/// 胴体の位置を設定する
	/// </summary>
	/// <param name="pos"></param>
	void SetDrop(const Vector2& pos);

#ifdef _DEBUG
	void Debug_Gui();
#endif

	void SetIsUiMove(bool isMove) { isUiMove_ = isMove; }

private:

	AdjustmentItem* adjust_;
	std::unique_ptr<Sprite> maxBody_UI_ = nullptr;

	std::unique_ptr<Sprite> gaugeBack_UI_ = nullptr;
	std::unique_ptr<Sprite> gaugeFront_UI_ = nullptr;

	std::unique_ptr<Sprite> head_UI_ = nullptr;
	std::unique_ptr<Sprite> tail_UI_ = nullptr;

	std::list<BodyUIData> body_UI_List_;

	float interval_bodyUI_;
	
	Vector2 uiPos_;

	// Parameter
	float time_;
	float moveTime_ = 2.0f;

	Vector2 fadeInStartPos_;
	Vector2 fadeOutPos_;

	Vector2 backPos_UI_;
	Vector2 frontPos_UI_;

	Vector2 frontSize_UI_;

	Vector2 headUIPos_;
	Vector2 tailUIPos_;

	// フラグ
	bool isUiMove_;
	bool isFadeIn_;

	int preCount_;

	// -------------------------------------------------
	// ↓ UIEffect用
	// -------------------------------------------------

	std::list<EffectData> effectList_;
	float effectMoveTime_;

	std::unique_ptr<Sprite> bodyReleseEffect_;
	Vector2 dropVelocity_;
	Vector2 dropStartVelocity_;
	Vector2 dropEndVelocity_;

	Vector4 dropBodyColor_;
	float dropBodyAlpa_;

	float dropDownCount_;
	float dropDownTime_;

	float dropRotate_;

	bool isDrop_;
};

