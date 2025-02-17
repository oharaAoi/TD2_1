#include "PlayerSpeedCounter.h"
#include "Engine/Math/Easing.h"
#include "Engine/Audio/AudioPlayer.h"

PlayerSpeedCounter::PlayerSpeedCounter(){}

PlayerSpeedCounter::~PlayerSpeedCounter(){}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerSpeedCounter::Init(){
	timeDrawScale_ = { 0.5f, 0.5f };
	backSprite_ = Engine::CreateSprite("Mater.png");
	taniSprite_ = Engine::CreateSprite("tani.png");
	needleSprite_ = Engine::CreateSprite("MaterStaple.png");
	// 200, 520

	adjustmentItem_ = AdjustmentItem::GetInstance();
	// 登録
	adjustmentItem_->AddItem(groupName_, "backSprite_position", backSprite_->GetCenterPos());
	adjustmentItem_->AddItem(groupName_, "numberoriginPos_", numberOriginPos_);
	adjustmentItem_->AddItem(groupName_, "taniSprite_position", taniSprite_->GetCenterPos());
	adjustmentItem_->AddItem(groupName_, "needleSprite_position", needleSprite_->GetCenterPos());

	AdaptAdjustmentItem();

	currentDigit_ = 3;
	maxDigit_ = currentDigit_;

	for(int oi = 1; oi <= (int)maxDigit_; ++oi) {
		auto& sprite = UI_speed_.emplace_back(Engine::CreateSprite("number.png"));

		sprite->SetRectRange(numberSpriteSize_);
		sprite->SetTextureSize(numberSpriteSize_);
		sprite->SetTextureCenterPos({ numberOriginPos_.x - ((oi - 2) * (numberSpriteSize_.x - 10)) ,numberOriginPos_.y });
		sprite->SetLeftTop(CalculationSpriteLT(IntegerCount(static_cast<float>(0.0f), oi)));
		sprite->SetScale(timeDrawScale_);
		sprite->Update();
	}

	// -------------------------------------------------
	// ↓ speedMax
	// -------------------------------------------------

	speedMaxPos_ = { -700, drawHeight };
	speedMaxUI_ = Engine::CreateSprite("speedMax.png");
	speedMaxUI_->SetTextureCenterPos(speedMaxPos_);
	speedMaxUI_->SetScale(Vector2(0.7f, .7f));

	isFadeIn_ = true;
	time_ = 0.0f;
	moveTime_ = 1.5f;

	fadeInStartPos_ = { -700, drawHeight };
	fadeOutPos_ = { 2000, drawHeight };
	isUiMove_ = false;
	isFinish_ = false;
	isFadeIn_ = true;

	// -------------------------------------------------
	// ↓ announce
	// -------------------------------------------------
	speedSprite_ = Engine::CreateSprite("speed.png");
	speedSprite_->SetScale({0.6f, 0.6f});
	speedSprite_->SetCenterPos({ -500, 160.0f });
	speedSprite_->Update();

	percentSprite_ = Engine::CreateSprite("percent2.png");
	percentSprite_->SetCenterPos({ -500, 160.0f });
	percentSprite_->Update();

	for (int oi = 0; oi < 2; ++oi) {
		speedAnnounceNumber_[oi] = Engine::CreateSprite("number.png");
		speedAnnounceNumber_[oi]->SetCenterPos({ -400, 160.0f });
		speedAnnounceNumber_[oi]->SetRectRange(numberSpriteSize_);
		speedAnnounceNumber_[oi]->SetTextureSize(numberSpriteSize_);
		//speedAnnounceNumber_[oi]->SetTextureCenterPos({ numberOriginPos_.x - ((oi - 2) * (numberSpriteSize_.x - 10)) ,numberOriginPos_.y });
		speedAnnounceNumber_[oi]->SetLeftTop(CalculationSpriteLT(IntegerCount(static_cast<float>(0.0f), oi)));
		speedAnnounceNumber_[oi]->Update();
	}
	
	speedRaitoState_ = SpeedRaitoState::Raito_0;
	isAnnounce_ = false;

	announceTime_ = 0.0f;
	announceMoveTime_ = 1.5f;

	announcePos_ = { -200, 100.0f };
	announceFadeInStartPos_ = { -200, 100.0f };
	announceFadeOutPos_ = { 2000, 100.0f };
	isAnnounceUiMove_ = false;
	isAnnounceFinish_ = false;
	isAnnounceFadeIn_ = true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerSpeedCounter::Update(float speed, float raito, float alpha, bool isPlayerFlying){

	backSprite_->SetColor({ 1.0f,1.0f,1.0f,alpha });
	taniSprite_->SetColor({ 1.0f,1.0f,1.0f,alpha });
	needleSprite_->SetColor({ 1.0f,1.0f,1.0f,alpha });

	easeRatio = std::lerp(easeRatio, raito, easeT);
	backSprite_->Update();
	taniSprite_->Update();
	needleSprite_->SetRotate(std::lerp(0.0f, needleAngleMax_, easeRatio));
	needleSprite_->Update();

	// -------------------------------------------------
	// ↓ 現在のspeedの割合を出す
	// -------------------------------------------------

	SpeedRaitoUpdate(speed);
	SpeedAnnounceMove();

	// -------------------------------------------------
	// ↓ speedMaxになる時の計算を行う
	// -------------------------------------------------
	if (!isPlayerFlying) {
		if (speed == maxSpeed_) {
			if (preSpeed_ != maxSpeed_) {
				isUiMove_ = true;
				isFinish_ = false;
				AudioPlayer::SinglShotPlay("missionClear.mp3", 0.3f);
			}
		}
	}

	for(int oi = 0; oi < UI_speed_.size(); ++oi) {
		UI_speed_[oi]->SetColor({ 1.0f,1.0f,1.0f,alpha });
		UI_speed_[oi]->SetLeftTop(CalculationSpriteLT(IntegerCount(speed, oi + 1)));
		UI_speed_[oi]->Update();
	}

	SpeedMaxUpdate();

	preSpeed_ = speed;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerSpeedCounter::Draw() const{
	if (!isUiMove_) {
		speedSprite_->Draw();
		percentSprite_->Draw();
		for (int oi = 0; oi < 2; ++oi) {
			speedAnnounceNumber_[oi]->Draw();
		}
	}

	backSprite_->Draw();
	//taniSprite_->Draw();
	needleSprite_->Draw();
	for(int oi = 0; oi < UI_speed_.size(); ++oi) {
		UI_speed_[oi]->Draw();
	}

	if (isFinish_) { return; }
	speedMaxUI_->Draw();
}

//===========================================================================================//
//
// Speedの割合に関する処理
//
//===========================================================================================//

void PlayerSpeedCounter::SpeedRaitoUpdate(float speed) {
	if (isTutorial_) {
		return;
	}
	
	float speedRaito = speed / maxSpeed_;

	bool isUp = false;

	if (speedRaito < firstRaito_) {
		speedRaitoState_ = SpeedRaitoState::Raito_0;
		announceTime_ = 0.0f;
	} else if (speedRaito < secondRaito_) {
		if (speedRaitoState_ != SpeedRaitoState::Raito_70) {
			speedRaitoState_ = SpeedRaitoState::Raito_30;
			announceTime_ = 0.0f;
		}
	}

	if (speedRaito >= firstRaito_ && speedRaitoState_ == SpeedRaitoState::Raito_0) {
		if (speedRaitoState_ < SpeedRaitoState::Raito_30) {
			isUp = true;
		}
	} else if (speedRaito >= secondRaito_ && speedRaitoState_ == SpeedRaitoState::Raito_30) {
		if (speedRaitoState_ < SpeedRaitoState::Raito_70) {
			isUp = true;
		}
	}

	if (isUp) {
		if (speedRaitoState_ < SpeedRaitoState::Raito_30) {
			speedRaitoState_ = SpeedRaitoState::Raito_30;
			isAnnounceUiMove_ = true;
			isAnnounceFinish_ = false;
			isAnnounceFadeIn_ = true;
			announceTime_ = 0.0f;
			announcePos_ = { -200, 100.0f };
		} else if(speedRaitoState_ < SpeedRaitoState::Raito_70) {
			speedRaitoState_ = SpeedRaitoState::Raito_70;
			isAnnounceUiMove_ = true;
			isAnnounceFinish_ = false;
			isAnnounceFadeIn_ = true;
			announceTime_ = 0.0f;
			announcePos_ = { -200, 100.0f };
		} else if (speedRaitoState_ < SpeedRaitoState::Raito_100) {
			speedRaitoState_ = SpeedRaitoState::Raito_100;
			isAnnounceUiMove_ = false;
			isAnnounceFinish_ = false;
			isAnnounceFadeIn_ = true;
			announceTime_ = 0.0f;
			announcePos_ = { -200, 100.0f };
		}
	}

	speedSprite_->SetCenterPos(announcePos_ + speedLocalPos_);
	percentSprite_->SetCenterPos(announcePos_ + percentLocalPos_);

	speedSprite_->Update();
	percentSprite_->Update();
	for (int oi = 0; oi < 2; ++oi) {
		if (speedRaitoState_ == SpeedRaitoState::Raito_30) {
			speedAnnounceNumber_[oi]->SetLeftTop(CalculationSpriteLT(IntegerCount(40.0f, oi + 1)));
		} else if(speedRaitoState_ == SpeedRaitoState::Raito_70) {
			speedAnnounceNumber_[oi]->SetLeftTop(CalculationSpriteLT(IntegerCount(70.0f, oi + 1)));
		}
		speedAnnounceNumber_[oi]->SetCenterPos(announcePos_ + numberLocalPos_ + (numberSpriteDivision_ * (1 - oi)));
		speedAnnounceNumber_[oi]->Update();
	}
}

void PlayerSpeedCounter::SpeedAnnounceMove() {
	if (isAnnounceFinish_) { return; }
	if (!isAnnounceUiMove_) { return; }

	// fadeがtrueだったら画面外から画面ないへ
	announceTime_ += GameTimer::DeltaTime();
	float t = announceTime_ / announceMoveTime_;
	if (isAnnounceFadeIn_) {
		announcePos_ = Vector2::Lerp(announceFadeInStartPos_, Vector2(640, announceFadeInStartPos_.y), EaseOutElastic(t));
	} else {
		announcePos_ = Vector2::Lerp(Vector2(640, announceFadeInStartPos_.y), announceFadeOutPos_, EaseInOutBack(t));
	}

	// 時間を過ぎたら
	if (announceTime_ >= announceMoveTime_) {
		announceTime_ = 0.0f;

		if (!isAnnounceFadeIn_) {
			isAnnounceFinish_ = true;
		}
		isAnnounceFadeIn_ = !isAnnounceFadeIn_;
	}
}

//===========================================================================================//
//
// speedMaxに関する処理
//
//===========================================================================================//

void PlayerSpeedCounter::SpeedMaxUpdate() {
	if (isFinish_) { return; }
	if (!isUiMove_) { return; }
	SpeedMaxMove();

	speedMaxUI_->SetTextureCenterPos(speedMaxPos_);
	speedMaxUI_->Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerSpeedCounter::SpeedMaxMove() {
	// fadeがtrueだったら画面外から画面ないへ
	time_ += GameTimer::DeltaTime();
	float t = time_ / moveTime_;
	if (isFadeIn_) {
		speedMaxPos_ = Vector2::Lerp(fadeInStartPos_, Vector2(640, fadeInStartPos_.y), EaseOutElastic(t));
	} else {
		speedMaxPos_ = Vector2::Lerp(Vector2(640, fadeInStartPos_.y), fadeOutPos_, EaseInOutBack(t));
	}

	// 時間を過ぎたら
	if (time_ >= moveTime_) {
		time_ = 0.0f;

		if (!isFadeIn_) {
			isFinish_ = true;
		}
		isFadeIn_ = !isFadeIn_;
	}
}

//===========================================================================================//
//
// その他処理
//
//===========================================================================================//

float PlayerSpeedCounter::DegitCount(float value){
	if(value == 0.0f) { return 0; }
	// 桁数 = log10(値) + 1(std::log10はvalueが10以下の時は0が返される)
	return static_cast<float>(std::floor(std::log10(value)) + 1);
}

float PlayerSpeedCounter::IntegerCount(float value, int n){
	if(value <= 0.0f) {
		return 0.0f;
	}
	// n桁目を取得するために数値を10で割ったあまりから割り出す
	int num = (static_cast<int>(value) / static_cast<int>(std::pow(10, n - 1)));
	int result = num % 10;
	return static_cast<float>(result);
}

Vector2 PlayerSpeedCounter::CalculationSpriteLT(float value){
	if(value == 0) {
		return { 0.0f, 0.0f };
	} else if(value == 1) {
		return { numberSpriteSize_.x , 0.0f };
	} else if(value == 2) {
		return { numberSpriteSize_.x * 2.0f, 0.0f };
	} else if(value == 3) {
		return { numberSpriteSize_.x * 3.0f, 0.0f };
	} else if(value == 4) {
		return { numberSpriteSize_.x * 4.0f, 0.0f };
	} else if(value == 5) {
		return { numberSpriteSize_.x * 5.0f, 0.0f };
	} else if(value == 6) {
		return { numberSpriteSize_.x * 6.0f, 0.0f };
	} else if(value == 7) {
		return { numberSpriteSize_.x * 7.0f, 0.0f };
	} else if(value == 8) {
		return { numberSpriteSize_.x * 8.0f , 0.0f };
	} else {
		return { numberSpriteSize_.x * 9.0f , 0.0f };
	}
}

//void PlayerSpeedCounter::CreateNewDigite(std::vector<std::unique_ptr<Sprite>>& array, float value, uint32_t digite, const Vector2& origin) {
//}

//===========================================================================================//
//
// Debug処理
//
//===========================================================================================//
#ifdef _DEBUG
#include <externals/imgui/imgui.h>
void PlayerSpeedCounter::Debug_Gui(){
	if(ImGui::TreeNode("PlayerSpeedCounterUI")) {
		Vector2 backPos = backSprite_->GetCenterPos();
		Vector2 taniPos = taniSprite_->GetCenterPos();
		Vector2 needlePos = needleSprite_->GetCenterPos();
		float needleAngle = needleSprite_->GetRotate();

		ImGui::DragFloat2("backPos", &backPos.x, 1.0f);
		ImGui::DragFloat2("taniPos", &taniPos.x, 1.0f);
		ImGui::DragFloat2("needlePos ", &needlePos.x, 1.0f);
		ImGui::DragFloat("needleRotate", &needleAngle, 0.01f);
		ImGui::DragFloat2("numberOriginPos", &numberOriginPos_.x, 1.0f);

		ImGui::BulletText("speedAnnounce");
		ImGui::DragFloat2("announcePos", &announcePos_.x, 1.0f);
		ImGui::DragFloat2("speedLocalPos", &speedLocalPos_.x, 1.0f);
		ImGui::DragFloat2("numberLocalPos", &numberLocalPos_.x, 1.0f);
		ImGui::DragFloat2("percentLocalPos", &percentLocalPos_.x, 1.0f);
		ImGui::DragFloat2("numberSpriteDivision", &numberSpriteDivision_.x, 1.0f);

		for(int oi = 0; oi < (int)maxDigit_; ++oi) {
			UI_speed_[oi]->SetTextureCenterPos({ numberOriginPos_.x - ((oi - 1) * (numberSpriteSize_.x - 10)) ,numberOriginPos_.y });
		}

		backSprite_->SetCenterPos(backPos);
		taniSprite_->SetCenterPos(taniPos);
		needleSprite_->SetCenterPos(needlePos);
		needleSprite_->SetRotate(needleAngle);

		if(ImGui::Button("ReAdapt")) {
			AdaptAdjustmentItem();
			UI_speed_.clear();
			for(int oi = 1; oi <= (int)maxDigit_; ++oi) {
				auto& sprite = UI_speed_.emplace_back(Engine::CreateSprite("number.png"));

				sprite->SetRectRange(numberSpriteSize_);
				sprite->SetTextureSize(numberSpriteSize_);
				sprite->SetTextureCenterPos({ numberOriginPos_.x - ((oi - 1) * (numberSpriteSize_.x - 10)) ,numberOriginPos_.y });

				sprite->SetLeftTop(CalculationSpriteLT(IntegerCount(static_cast<float>(0.0f), oi)));
				sprite->SetScale(timeDrawScale_);
				sprite->Update();
			}
		}
		ImGui::TreePop();
	}
}
#endif // _DEBUG

void PlayerSpeedCounter::AdaptAdjustmentItem(){
	backSprite_->SetTextureCenterPos(adjustmentItem_->GetValue<Vector2>(groupName_, "backSprite_position"));
	taniSprite_->SetTextureCenterPos(adjustmentItem_->GetValue<Vector2>(groupName_, "taniSprite_position"));
	needleSprite_->SetTextureCenterPos(adjustmentItem_->GetValue<Vector2>(groupName_, "needleSprite_position"));
	numberOriginPos_ = adjustmentItem_->GetValue<Vector2>(groupName_, "numberoriginPos_");
}