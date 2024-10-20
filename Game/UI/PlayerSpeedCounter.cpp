#include "PlayerSpeedCounter.h"

PlayerSpeedCounter::PlayerSpeedCounter() {
}

PlayerSpeedCounter::~PlayerSpeedCounter() {
}

void PlayerSpeedCounter::Init() {
	backSprite_ = Engine::CreateSprite("speedMeterBack.png");
	taniSprite_ = Engine::CreateSprite("tani.png");
	// 200, 520

	adjustmentItem_ = AdjustmentItem::GetInstance();
	// 登録
	adjustmentItem_->AddItem(groupName_, "backSprite_position", backSprite_->GetCenterPos());
	adjustmentItem_->AddItem(groupName_, "numberoriginPos_", numberOriginPos_);
	adjustmentItem_->AddItem(groupName_, "taniSprite_position", taniSprite_->GetCenterPos());

	AdaptAdjustmentItem();

	currentDigit_ = 3;
	maxDigit_ = currentDigit_;

	for (int oi = 1; oi <= maxDigit_; ++oi) {
		auto& sprite = UI_speed_.emplace_back(Engine::CreateSprite("number.png"));

		sprite->SetRectRange(numberSpriteSize_);
		sprite->SetTextureSize(numberSpriteSize_);
		sprite->SetTextureCenterPos({ numberOriginPos_.x - ((oi - 1) * numberSpriteSize_.x) ,numberOriginPos_.y });

		sprite->SetLeftTop(CalculationSpriteLT(IntegerCount(static_cast<float>(0.0f), oi)));
		sprite->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
		sprite->Update();
	}
}

void PlayerSpeedCounter::Update(float speed) {
	backSprite_->Update();
	taniSprite_->Update();

	for (int oi = 0; oi < UI_speed_.size(); ++oi) {
		UI_speed_[oi]->SetLeftTop(CalculationSpriteLT(IntegerCount(speed, oi + 1)));
		UI_speed_[oi]->Update();
	}
}

void PlayerSpeedCounter::Draw() const {
	backSprite_->Draw();
	taniSprite_->Draw();
	for (int oi = 0; oi < UI_speed_.size(); ++oi) {
		UI_speed_[oi]->Draw();
	}
}

float PlayerSpeedCounter::DegitCount(float value) {
	if (value == 0.0f) { return 0; }
	// 桁数 = log10(値) + 1(std::log10はvalueが10以下の時は0が返される)
	return static_cast<float>(std::floor(std::log10(value)) + 1);
}

float PlayerSpeedCounter::IntegerCount(float value, int n) {
	if (value <= 0.0f) {
		return 0.0f;
	}
	// n桁目を取得するために数値を10で割ったあまりから割り出す
	int num = (static_cast<int>(value) / static_cast<int>(std::pow(10, n - 1)));
	int result = num % 10;
	return static_cast<float>(result);
}

Vector2 PlayerSpeedCounter::CalculationSpriteLT(float value) {
	if (value == 1) {
		return { 0.0f, 0.0f };
	} else if (value == 2) {
		return { numberSpriteSize_.x , 0.0f };
	} else if (value == 3) {
		return { numberSpriteSize_.x * 2.0f, 0.0f };
	} else if (value == 4) {
		return { 0.0f, numberSpriteSize_.y };
	} else if (value == 5) {
		return { numberSpriteSize_.x, numberSpriteSize_.y };
	} else if (value == 6) {
		return { numberSpriteSize_.x * 2.0f, numberSpriteSize_.y };
	} else if (value == 7) {
		return { 0.0f, numberSpriteSize_.y * 2.0f };
	} else if (value == 8) {
		return { numberSpriteSize_.x, numberSpriteSize_.y * 2.0f };
	} else if (value == 9) {
		return { numberSpriteSize_.x * 2.0f, numberSpriteSize_.y * 2.0f };
	} else {
		return { 0.0f, numberSpriteSize_.y * 3.0f };
	}
}

void PlayerSpeedCounter::CreateNewDigite(std::vector<std::unique_ptr<Sprite>>& array, float value, uint32_t digite, const Vector2& origin) {
}

#ifdef _DEBUG
#include <externals/imgui/imgui.h>
void PlayerSpeedCounter::Debug_Gui() {
	if (ImGui::TreeNode("PlayerSpeedCounterUI")) {
		Vector2 backPos = backSprite_->GetCenterPos();
		Vector2 taniPos = taniSprite_->GetCenterPos();

		ImGui::DragFloat2("backPos", &backPos.x, 1.0f);
		ImGui::DragFloat2("taniPos", &taniPos.x, 1.0f);
		ImGui::DragFloat2("numberOriginPos", &numberOriginPos_.x, 1.0f);

		backSprite_->SetCenterPos(backPos);
		taniSprite_->SetCenterPos(taniPos);

		if (ImGui::Button("ReAdapt")) {
			AdaptAdjustmentItem();
			UI_speed_.clear();
			for (int oi = 1; oi <= maxDigit_; ++oi) {
				auto& sprite = UI_speed_.emplace_back(Engine::CreateSprite("number.png"));

				sprite->SetRectRange(numberSpriteSize_);
				sprite->SetTextureSize(numberSpriteSize_);
				sprite->SetTextureCenterPos({ numberOriginPos_.x - ((oi - 1) * numberSpriteSize_.x) ,numberOriginPos_.y });

				sprite->SetLeftTop(CalculationSpriteLT(IntegerCount(static_cast<float>(0.0f), oi)));
				sprite->Update();
			}
		}
		ImGui::TreePop();
	}
}
#endif // _DEBUG

void PlayerSpeedCounter::AdaptAdjustmentItem() {
	backSprite_->SetTextureCenterPos(adjustmentItem_->GetValue<Vector2>(groupName_, "backSprite_position"));
	taniSprite_->SetTextureCenterPos(adjustmentItem_->GetValue<Vector2>(groupName_, "taniSprite_position"));
	numberOriginPos_ = adjustmentItem_->GetValue<Vector2>(groupName_, "numberoriginPos_");
}