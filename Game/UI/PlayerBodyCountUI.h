#pragma once
#include <memory>
#include <string>
#include <list>
#include "Engine/Engine.h"
#include "Engine/2d/Sprite.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/Math/Easing.h"
#include "Engine/Math/Vector4.h"

enum BodyRaitoState {
	Raito_Zero,
	Raito_3,
	Raito_7,
	Raito_1,
};

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
	void Update(int playerBodyCount, bool isFlying);
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

	// ------------------- bodyの割合に関する処理 ------------------- //

	void BodyRaitoUpdate(float speed, bool isFlying);

	void BodyAnnounceMove();

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


#ifdef _DEBUG
	void Debug_Gui();
#endif

	void SetIsUiMove(bool isMove) { isUiMove_ = isMove; }

	void SetIsTutorial(bool isTutorial) {isTutorial_ = isTutorial;}

private:

	AdjustmentItem* adjust_;
	std::unique_ptr<Sprite> maxBody_UI_ = nullptr;

	std::unique_ptr<Sprite> gaugeBack_UI_ = nullptr;
	std::unique_ptr<Sprite> gaugeFront_UI_ = nullptr;

	std::unique_ptr<Sprite> head_UI_ = nullptr;
	std::unique_ptr<Sprite> tail_UI_ = nullptr;
	
	std::list<BodyUIData> body_UI_List_;
	std::list<BodyUIData> backBody_UI_List_;

	uint32_t nowBodyCount_ = 1;

	float interval_bodyUI_;
	
	Vector2 maxUIPos;

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

	Vector2 lastBodyPos_;

	Vector2 bodyScale_ = { .7f, .7f };

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

	// -------------------------------------------------
	// ↓ bodyのアナウンス
	// -------------------------------------------------

	BodyRaitoState BodyRaitoState_ = Raito_Zero;
	bool isAnnounce_ = false;

	std::unique_ptr<Sprite> bodySprite_;
	std::unique_ptr<Sprite> percentSprite_;
	std::unique_ptr<Sprite> bodyAnnounceNumber_[2];

	Vector2 announcePos_;
	Vector2 bodyLocalPos_ = { -100.0f, 0.0f };
	Vector2 percentLocalPos_ = { 125.0f, 0.0f };
	Vector2 numberLocalPos_;
	Vector2 numberSpriteDivision_ = { 64.0f, 0.0f };

	// Parameter
	float announceTime_;
	float announceMoveTime_ = 1.8f;

	Vector2 announceFadeInStartPos_;
	Vector2 announceFadeOutPos_;
	Vector2 announceBodyMaxPos_;

	// フラグ
	bool isAnnounceUiMove_;
	bool isAnnounceFadeIn_;
	bool isAnnounceFinish_;

	bool isTutorial_ = false;

	Vector2 numberSpriteSize_ = { 48.0f, 72.0f };
	Vector2 numberOriginPos_;

	float bodyRaito_ = 0.0f;
	float firstRaito_ = .3f;
	float secondRaito_ = .7f;

};

