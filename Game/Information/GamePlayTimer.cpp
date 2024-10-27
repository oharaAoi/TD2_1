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

	numberInterval_ = 36.0f;

	originPos_ = { 910.0f, 100.0f };
	numberSpriteScale_ = { 0.8f, 0.8f };

	clock_ = Engine::CreateSprite("timer.png");
	clock_->SetScale({0.35f, 0.35f});

	adjustItem_ = AdjustmentItem::GetInstance();
	groupName_ = "GamePlayTimer";
	adjustItem_->AddItem(groupName_, "clock_position", clock_->GetCenterPos());
	adjustItem_->AddItem(groupName_, "numberOriginPos", originPos_);
	adjustItem_->AddItem(groupName_, "numberInterval", numberInterval_);
	adjustItem_->AddItem(groupName_, "numberSpriteScale", numberSpriteScale_);

	// 調整項目の適応
	AdaptAdjustmentItem();

	// -------------------------------------------------
	// ↓ 制限時間のUIのSpriteを作成する
	// -------------------------------------------------
	int digit = DegitCount(gameTimeLimit_);
	for (int oi = 1; oi <= digit; ++oi) {
		auto& sprite = limitTimeUI_.emplace_back(Engine::CreateSprite("number.png"));
		sprite->SetRectRange(numberSpriteSize_);
		sprite->SetTextureCenterPos(Vector2{ originPos_.x - (numberInterval_ * static_cast<float>(oi - 1)), originPos_.y });
		sprite->SetTextureSize(numberSpriteSize_);
		sprite->SetLeftTop(CalculationSpriteLT(IntegerCount(static_cast<int>(gameTimeLimit_), oi)));
		sprite->SetScale(numberSpriteScale_);
		sprite->Update();
	}

	timeleft10s_ = std::make_unique<AudioPlayer>();
	timeleft10s_->Init("timeRemaining.mp3");

	timeleft60s_ = std::make_unique<AudioPlayer>();
	timeleft60s_->Init("timeRemaining.mp3");

	// -------------------------------------------------
	// ↓ 時間を足すUIのSpriteを作成する
	// -------------------------------------------------
	addTimeSprite_ = Engine::CreateSprite("missionClearAddTime.png");
	addTimeSprite_->SetCenterPos(clock_->GetCenterPos());
	addTimeSprite_->SetScale({ 0.5f, 0.5f });
	isAddTime_ = false;
	addClockMoveCount_ = 0.0f;
	addClockMoveTime_ = 1.0f;

	addTimeEndPos_ = clock_->GetCenterPos();
	addTimeEndPos_.x += -100.0f;
	addClockColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	alpa_ = 0.0f;

	isMove_ = false;
	isFadeIn_ = true;

	startPos_ = {-600.0f, 250.0f };
	endPos_ = { 2000.0f, 250.0f };
	time_ = 0;
	moveTime_ = 1.5f;
	timeleftUI_ = Engine::CreateSprite("timer60.png");

	timeleftUI_->SetCenterPos(startPos_);
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GamePlayTimer::Update(bool isPlayerFlying) {
	clock_->Update();
	gameTimer_ -= GameTimer::DeltaTime();
	if (gameTimer_ < 0.0f) {
		gameTimer_ = 0.0f;
		for(int oi = 0; oi < limitTimeUI_.size(); ++oi) {
			limitTimeUI_[oi]->SetLeftTop(CalculationSpriteLT(IntegerCount(static_cast<int>(gameTimer_), oi + 1)));
			limitTimeUI_[oi]->Update();
		}
	} else
	{
		for(int oi = 0; oi < limitTimeUI_.size(); ++oi) {
			limitTimeUI_[oi]->SetLeftTop(CalculationSpriteLT(IntegerCount(static_cast<int>(gameTimer_ + 1), oi + 1)));
			limitTimeUI_[oi]->Update();
		}
	}

	// タイムアップ10秒前
	if (std::floor(gameTimer_) == 10.0f) {
		timeleftUI_->SetTexture("timer10.png");
		timeleft10s_->Play(false, 0.5f, true);
		isMove_ = true;
	}
	// タイムアップ60秒前
	if (std::floor(gameTimer_) == 60.0f) {
		timeleftUI_->SetTexture("timer60.png");
		timeleft60s_->Play(false, 0.5f, true);
		isMove_ = true;
	}

	// Playerが飛んでいたら
	if (!isPlayerFlying) {
		// 制限時間を超えたら
		if (gameTimer_ <= 0.0f) {
			isFinish_ = true;
			//AudioPlayer::SinglShotPlay("timeUp.wav", 0.6f);
		}
	} else { // オーバータイムに入る
		isOverTime_ = true;

		// 今飛んでいなくて前飛んでいたら
		if (!isPlayerFlying && isPreFlying_) {
			isFinish_ = true;
			//AudioPlayer::SinglShotPlay("timeUp.wav", 0.6f);
		}
	}

	if(isFinish_){
		outgameTime_ += GameTimer::DeltaTime();
	}

	if (isMove_) {
		SpriteMove();
		timeleftUI_->SetCenterPos(uiPos_);
		timeleftUI_->Update();
	}

	if (isAddTime_) {
		AddClockMove();
		addTimeSprite_->SetColor(addClockColor_);
		addTimeSprite_->Update();
	}

#ifdef _DEBUG
	if(Input::IsTriggerKey(DIK_0)) 
	{
		gameTimer_ = 3;
	}
#endif // _DEBUG

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GamePlayTimer::Draw() const {
	clock_->Draw();
	for (int oi = 0; oi < limitTimeUI_.size(); ++oi) {
		limitTimeUI_[oi]->Draw();
	}

	if (isAddTime_) {
		addTimeSprite_->Draw();
	}

	timeleftUI_->Draw();
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
	if (value == 0) {
		return { 0.0f, 0.0f };
	} else if (value == 1) {
		return { numberSpriteSize_.x , 0.0f };
	} else if (value == 2) {
		return { numberSpriteSize_.x * 2.0f, 0.0f };
	} else if (value == 3) {
		return { numberSpriteSize_.x * 3.0f, 0.0f };
	} else if (value == 4) {
		return { numberSpriteSize_.x * 4.0f, 0.0f };
	} else if (value == 5) {
		return { numberSpriteSize_.x * 5.0f, 0.0f };
	} else if (value == 6) {
		return { numberSpriteSize_.x * 6.0f, 0.0f };
	} else if (value == 7) {
		return { numberSpriteSize_.x * 7.0f, 0.0f };
	} else if (value == 8) {
		return { numberSpriteSize_.x * 8.0f , 0.0f };
	} else {
		return { numberSpriteSize_.x * 9.0f , 0.0f };
	}
}

void GamePlayTimer::AdaptAdjustmentItem() {
	clock_->SetTextureCenterPos(adjustItem_->GetValue<Vector2>(groupName_, "clock_position"));
	originPos_ = adjustItem_->GetValue<Vector2>(groupName_, "numberOriginPos");
	numberInterval_ = adjustItem_->GetValue<float>(groupName_, "numberInterval");
	numberSpriteScale_ = adjustItem_->GetValue<Vector2>(groupName_, "numberSpriteScale");
}

void GamePlayTimer::SpriteMove() {
	// fadeがtrueだったら画面外から画面ないへ
	time_ += GameTimer::DeltaTime();
	float t = time_ / moveTime_;
	if (isFadeIn_) {
		uiPos_ = Vector2::Lerp(startPos_, Vector2(640, startPos_.y), EaseOutElastic(t));
	} else {
		uiPos_ = Vector2::Lerp(Vector2(640, startPos_.y), endPos_, EaseInOutBack(t));
	}

	// 時間を過ぎたら
	if (time_ >= moveTime_) {
		time_ = 0.0f;

		if (!isFadeIn_) {
			isMove_ = false;
			time_ = 0.0f;
		}

		isFadeIn_ = !isFadeIn_;
	}
}

void GamePlayTimer::AddTime(float time) {
	if (isOverTime_) {
		return;
	}

	gameTimer_ += time;
	isAddTime_ = true;

	isAppearance_ = true;
	addClockMoveCount_ = 0.0f;
}

void GamePlayTimer::AddClockMove() {
	addClockMoveCount_ += GameTimer::DeltaTime();
	float t = addClockMoveCount_ / addClockMoveTime_;

	if (isAppearance_) {
		Vector2 pos = addTimeSprite_->GetCenterPos();
		pos = Vector2::Lerp(clock_->GetCenterPos(), addTimeEndPos_, EaseOutExpo(t));
		addTimeSprite_->SetCenterPos(pos);

		alpa_ = std::lerp(0.0f, 1.0f, EaseOutExpo(t));
	} else {
		Vector2 pos = addTimeSprite_->GetCenterPos();
		pos = Vector2::Lerp(addTimeEndPos_, clock_->GetCenterPos(), EaseInQuart(t));
		addTimeSprite_->SetCenterPos(pos);

		alpa_ = std::lerp(1.0f, 0.0f, EaseInQuart(t));
	}

	addClockColor_.w = alpa_;

	if (addClockMoveCount_ > addClockMoveTime_) {
		addClockMoveCount_ = 0;

		if (!isAppearance_) {
			isAddTime_ = false;
		}

		isAppearance_ = !isAppearance_;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
void GamePlayTimer::Debug_Gui() {
	if (ImGui::TreeNode("GamePlayTimer")) {
		ImGui::Text("gameTimer_ : %f", gameTimer_);
		ImGui::DragFloat3("originPos", &originPos_.x, 0.1f);
		ImGui::DragFloat2("numberSpriteScale", &numberSpriteScale_.x, 0.1f);
		ImGui::DragFloat("inverval", &numberInterval_, 0.1f);
		for (int oi = 0; oi < limitTimeUI_.size(); ++oi) {
			limitTimeUI_[oi]->SetTextureCenterPos(Vector2{ originPos_.x - (numberInterval_ * static_cast<float>(oi)), originPos_.y });
			limitTimeUI_[oi]->SetScale(numberSpriteScale_);
		}
		clock_->Debug_Gui();

		if (ImGui::Button("AddTime")) {
			AddTime(10.0f);
		}

		if (ImGui::Button("ReAdapt")) {
			AdaptAdjustmentItem();
		}

		ImGui::TreePop();
	}
}
#endif