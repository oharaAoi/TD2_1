#include "GamePlayTimer.h"

GamePlayTimer::GamePlayTimer() {
}

GamePlayTimer::~GamePlayTimer() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GamePlayTimer::Init(float limit) {
	isFinish_ = false;
	gameTimer_ = limit;
	gameTimeLimit_ = limit;

	// -------------------------------------------------
	// ↓ 制限時間のUIのSpriteを作成する
	// -------------------------------------------------
	int digit = DegitCount(gameTimeLimit_);
	for (int oi = 1; oi <= digit; ++oi) {
		auto& sprite = limitTimeUI_.emplace_back(
			Engine::CreateSprite(Vector2{ 360 - (80.0f * static_cast<float>(oi)), 100.0f }, Vector2{ 64.0f, 64.0f })
		);
		sprite->SetTexture("number.png");
		sprite->SetRectRange({ 64.0f, 64.0f });
		sprite->SetLeftTop(CalculationSpriteLT(IntegerCount(static_cast<float>(gameTimeLimit_), oi)));
		sprite->Update();
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GamePlayTimer::Update() {
	gameTimer_ -= GameTimer::DeltaTime();
	if (gameTimer_ < 0.0f) {
		gameTimer_ = 0.0f;
	}

	for (int oi = 0 ; oi < limitTimeUI_.size(); ++oi) {
		limitTimeUI_[oi]->SetLeftTop(CalculationSpriteLT(IntegerCount(static_cast<int>(gameTimer_), oi + 1)));
		limitTimeUI_[oi]->Update();
	}

	// 制限時間を超えたら
	if (gameTimer_ <= 0.0f) {
		isFinish_ = true;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GamePlayTimer::Draw() const {
	for (int oi = 0; oi < limitTimeUI_.size(); ++oi) {
		limitTimeUI_[oi]->Draw();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　計測
//////////////////////////////////////////////////////////////////////////////////////////////////

void GamePlayTimer::Measurement() {
	isFinish_ = false;
	gameTimer_ = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　桁数を求める
//////////////////////////////////////////////////////////////////////////////////////////////////

int GamePlayTimer::DegitCount(float value) {
	if (value == 0.0f) { return 0; }
	// 桁数 = log10(値) + 1(std::log10はvalueが10以下の時は0が返される)
	return static_cast<int>(std::floor(std::log10(value)) + 1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　数値から任意の桁数の数を求める
//////////////////////////////////////////////////////////////////////////////////////////////////

int GamePlayTimer::IntegerCount(int value, int n) {
	// 整数部の桁数を計算
	/*int numDigits = static_cast<int>(std::log10(value)) + 1;
	if (numDigits - n < 0) {
		return 0;
	}*/
	// n桁目を取得するために数値を10で割ったあまりから割り出す
	int num = (value / static_cast<int>(std::pow(10, n - 1)));
	int result = num % 10;
	return result;
}

Vector2 GamePlayTimer::CalculationSpriteLT(int value) {
	if (value == 1) {
		return { 0.0f, 0.0f };
	} else if (value == 2) {
		return { 64.0f, 0.0f };
	} else if (value == 3) {
		return { 128.0f, 0.0f };
	} else if (value == 4) {
		return { 0.0f, 64.0f };
	} else if (value == 5) {
		return { 64.0f, 64.0f };
	} else if (value == 6) {
		return { 128.0f, 64.0f };
	} else if (value == 7) {
		return { 0.0f, 128.0f };
	} else if (value == 8) {
		return { 64.0f, 128.0f };
	} else if (value == 9) {
		return { 128.0f, 128.0f };
	} else {
		return { 0.0f, 192.0f };
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
void GamePlayTimer::Debug_Gui() {
	ImGui::Begin("GamePlayTimer");
	ImGui::Text("gameTimer_ : %f", gameTimer_);
	ImGui::End();
}
#endif