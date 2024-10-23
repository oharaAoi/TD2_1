#include "PlayerSpeedCounter.h"

PlayerSpeedCounter::PlayerSpeedCounter(){}

PlayerSpeedCounter::~PlayerSpeedCounter(){}

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
}

void PlayerSpeedCounter::Update(float speed, float raito, float alpha){

	backSprite_->SetColor({ 1.0f,1.0f,1.0f,alpha });
	taniSprite_->SetColor({ 1.0f,1.0f,1.0f,alpha });
	needleSprite_->SetColor({ 1.0f,1.0f,1.0f,alpha });

	easeRatio = std::lerp(easeRatio, raito, easeT);
	backSprite_->Update();
	taniSprite_->Update();
	needleSprite_->SetRotate(std::lerp(0.0f, needleAngleMax_, easeRatio));
	needleSprite_->Update();

	for(int oi = 0; oi < UI_speed_.size(); ++oi) {
		UI_speed_[oi]->SetColor({ 1.0f,1.0f,1.0f,alpha });
		UI_speed_[oi]->SetLeftTop(CalculationSpriteLT(IntegerCount(speed, oi + 1)));
		UI_speed_[oi]->Update();
	}
}

void PlayerSpeedCounter::Draw() const{
	backSprite_->Draw();
	//taniSprite_->Draw();
	needleSprite_->Draw();
	for(int oi = 0; oi < UI_speed_.size(); ++oi) {
		UI_speed_[oi]->Draw();
	}
}

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