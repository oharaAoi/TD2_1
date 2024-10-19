#include "FlyingTimerUI.h"

FlyingTimerUI::FlyingTimerUI() {
}

FlyingTimerUI::~FlyingTimerUI() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void FlyingTimerUI::Init() {
	currentPos_ = {800.0f, 100.0f};
	maxPos_ = { 800.0f, 200.0f };

	// -------------------------------------------------
	// ↓ 現在の飛行距離
	// -------------------------------------------------
	for (int oi = 1; oi <= 1; ++oi) {
		auto& sprite = UI_flyingLength_.emplace_back(Engine::CreateSprite("number.png"));

		sprite->SetRectRange(rangeSize_);
		sprite->SetTextureSize(rangeSize_);
		sprite->SetTextureCenterPos(currentPos_);
		
		sprite->SetLeftTop(CalculationSpriteLT(IntegerCount(static_cast<float>(0.0f), oi)));
		sprite->Update();
	}

	// -------------------------------------------------
	// ↓ 最大の飛行距離
	// -------------------------------------------------
	for (int oi = 1; oi <= 1; ++oi) {
		auto& sprite = UI_flyingMaxLength_.emplace_back(Engine::CreateSprite("number.png"));

		sprite->SetRectRange(rangeSize_);
		sprite->SetTextureSize(rangeSize_);
		sprite->SetTextureCenterPos(maxPos_);

		sprite->SetLeftTop(CalculationSpriteLT(IntegerCount(static_cast<float>(0.0f), oi)));
		sprite->Update();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void FlyingTimerUI::Update(float current, float max) {
	currentLength_ = current;
	maxLength_ = max;

	// -------------------------------------------------
	// ↓ 桁数の更新
	// -------------------------------------------------
	currentDigit_ = static_cast<uint32_t>(DegitCount(current));
	maxDigit_ = static_cast<uint32_t>(DegitCount(max));

	// 桁数と現在の配列の数が違う数だったら新しい桁を作成する
	if (currentDigit_ != UI_flyingLength_.size()) {
		CreateNewDigite(UI_flyingLength_, currentLength_, currentDigit_, currentPos_);
	}

	if (maxDigit_ != UI_flyingMaxLength_.size()) {
  		CreateNewDigite(UI_flyingMaxLength_, maxLength_, maxDigit_, maxPos_);
	}

	// -------------------------------------------------
	// ↓ UIの更新
	// -------------------------------------------------
	for (int oi = 0; oi < UI_flyingLength_.size(); ++oi) {
		UI_flyingLength_[oi]->SetLeftTop(CalculationSpriteLT(IntegerCount(currentLength_, oi + 1)));
		UI_flyingLength_[oi]->Update();
	}

	for (int oi = 0; oi < UI_flyingMaxLength_.size(); ++oi) {
		UI_flyingMaxLength_[oi]->SetLeftTop(CalculationSpriteLT(IntegerCount(maxLength_, oi + 1)));
		UI_flyingMaxLength_[oi]->Update();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void FlyingTimerUI::Draw() const {
	for (int oi = 0; oi < UI_flyingLength_.size(); ++oi) {
		UI_flyingLength_[oi]->Draw();
	}

	for (int oi = 0; oi < UI_flyingMaxLength_.size(); ++oi) {
		UI_flyingMaxLength_[oi]->Draw();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

float FlyingTimerUI::DegitCount(float value) {
	if (value == 0.0f) { return 0; }
	// 桁数 = log10(値) + 1(std::log10はvalueが10以下の時は0が返される)
	return static_cast<float>(std::floor(std::log10(value)) + 1);
}

float FlyingTimerUI::IntegerCount(float value, int n) {
	// n桁目を取得するために数値を10で割ったあまりから割り出す
	int num = (static_cast<int>(value) / static_cast<int>(std::pow(10, n - 1)));
	int result = num % 10;
	return static_cast<float>(result);
}

Vector2 FlyingTimerUI::CalculationSpriteLT(float value) {
	if (value == 1) {
		return { 0.0f, 0.0f };
	} else if (value == 2) {
		return { rangeSize_.x , 0.0f };
	} else if (value == 3) {
		return { rangeSize_.x * 2.0f, 0.0f };
	} else if (value == 4) {
		return { 0.0f, rangeSize_.y };
	} else if (value == 5) {
		return { rangeSize_.x, rangeSize_.y };
	} else if (value == 6) {
		return { rangeSize_.x * 2.0f, rangeSize_.y };
	} else if (value == 7) {
		return { 0.0f, rangeSize_.y * 2.0f };
	} else if (value == 8) {
		return { rangeSize_.x, rangeSize_.y * 2.0f };
	} else if (value == 9) {
		return { rangeSize_.x * 2.0f, rangeSize_.y * 2.0f };
	} else {
		return { 0.0f, rangeSize_.y * 3.0f };
	}
}

void FlyingTimerUI::CreateNewDigite(std::vector<std::unique_ptr<Sprite>>& array, float value,
									uint32_t digite, const Vector2& origin) {
	int createNum = (int)digite - static_cast<int>(array.size());
	if (createNum < 0) { return; }

	while (createNum != 0) {
		auto& newDigite = array.emplace_back(Engine::CreateSprite("number.png"));

		newDigite->SetTexture("number.png");
		newDigite->SetRectRange(rangeSize_);
		newDigite->SetTextureSize(rangeSize_);
		newDigite->SetTextureCenterPos(Vector2{ origin.x - (80.0f * static_cast<int>(array.size() - 1)), origin.y });
		newDigite->SetLeftTop(CalculationSpriteLT(IntegerCount(value, static_cast<int>(array.size() - 1))));

		createNum--;
	}
}

#ifdef _DEBUG
#include <externals/imgui/imgui.h>
void FlyingTimerUI::Debug_Gui() {
	if (ImGui::TreeNode("FlyingTimerUI")) {
		ImGui::DragFloat2("currentPos", &currentPos_.x,1.0f);
		ImGui::DragFloat2("maxPos", &maxPos_.x,1.0f);

		for (int oi = 0; oi < UI_flyingLength_.size(); ++oi) {
			UI_flyingLength_[oi]->SetTextureCenterPos(Vector2{ currentPos_.x - (80.0f * static_cast<float>(oi)), currentPos_.y });
		}

		for (int oi = 0; oi < UI_flyingMaxLength_.size(); ++oi) {
			UI_flyingMaxLength_[oi]->SetTextureCenterPos(Vector2{ maxPos_.x - (80.0f * static_cast<float>(oi)), maxPos_.y });
		}
		ImGui::TreePop();
	}
}
#endif // _DEBUG