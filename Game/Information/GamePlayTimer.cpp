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

	// 変数の初期化
	/*numberInterval_ = 36.0f;*/

	originPos_ = { 640.0f, 60.0f };
	//numberSpriteScale_ = { 0.8f, 0.8f };

	// gauge
	timeGauge_ = Engine::CreateSprite("TimeGaugeBar.png");
	timeGaugeOutSide_ = Engine::CreateSprite("TimeGauge2.png");
	
	// timeUpCount
	bigNumberUI_ = Engine::CreateSprite("BigNumber5.png");
	bigNumberUI_->SetTextureCenterPos({ 640.0f, 360.0f });

	// leftTime
	timeleft10s_ = std::make_unique<AudioPlayer>();
	timeleft10s_->Init("timeRemaining.mp3");

	timeleft60s_ = std::make_unique<AudioPlayer>();
	timeleft60s_->Init("timeRemaining.mp3");

	timeleftUI_ = Engine::CreateSprite("timer60.png");

	// -------------------------------------------------
	// ↓ 調整項目
	// -------------------------------------------------
	adjustItem_ = AdjustmentItem::GetInstance();
	groupName_ = "GamePlayTimer";
	adjustItem_->AddItem(groupName_, "timeGaugeOutSidePos", timeGaugeOutSide_->GetCenterPos());
	adjustItem_->AddItem(groupName_, "numberOriginPos", originPos_);
	adjustItem_->AddItem(groupName_, "timeGaugeBarOffset", timeGaugeBarOffset_);
	adjustItem_->AddItem(groupName_, "addTimeOffsetX", addTimeOffsetX_);
	adjustItem_->AddItem(groupName_, "addTimeStartPos", addTimeStartPos_);
	/*adjustItem_->AddItem(groupName_, "numberInterval", numberInterval_);
	adjustItem_->AddItem(groupName_, "numberSpriteScale", numberSpriteScale_);*/

	// 適応
	AdaptAdjustmentItem();

	// -------------------------------------------------
	// ↓ 時間を足すUIのSpriteを作成する
	// -------------------------------------------------
	addTimeSprite_ = Engine::CreateSprite("missionClearAddTime.png");
	addTimeSprite_->SetCenterPos(timeGauge_->GetCenterPos());
	addTimeSprite_->SetScale({ 0.5f, 0.5f });
	isAddTime_ = false;
	addClockMoveCount_ = 0.0f;
	addClockMoveTime_ = 1.0f;

	addTimeEndPos_ = timeGaugeOutSide_->GetCenterPos();
	addTimeEndPos_.x += addTimeOffsetX_;
	addClockColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	alpa_ = 0.0f;

	isMove_ = false;
	isFadeIn_ = true;

	isOverTime_ = false;

	startPos_ = { -600.0f, 250.0f };
	endPos_ = { 2000.0f, 250.0f };
	time_ = 0;
	moveTime_ = 1.5f;

	timeleftUI_->SetCenterPos(startPos_);

	timeGauge_->SetCenterPos(timeGaugeOutSide_->GetCenterPos() + timeGaugeBarOffset_);

	// -------------------------------------------------
	// ↓ scaleUp用の変数 
	// -------------------------------------------------

	bigNumberScale_ = {1.0f, 1.0f};
	scaleUpTime_ = 0.0f;
	bigNumberAlpha_ = 0.5f;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GamePlayTimer::Update(bool isPlayerFlying) {
	gameTimer_ -= GameTimer::DeltaTime();
	float raito = gameTimer_ / gameTimeLimit_;
	
	// 時間のゲージを変更する
	timeGaugeOutSide_->Update();

	timeGauge_->SetCenterPos(timeGaugeOutSide_->GetCenterPos() + timeGaugeBarOffset_);
	timeGauge_->SetUvDrawRange(Vector2(raito, 1.0f));

	timeGauge_->Update();

	// 5秒前になったら大きな時間を表示する
	if (gameTimer_ <= 5.0f) {

		if (!isOverTime_ && !isFinish_) {
			if (scaleUpTime_ == 0.0f) {
				AudioPlayer::SinglShotPlay("timeUpCount.mp3", 0.6f);
			}
		}

		scaleUpTime_ += GameTimer::DeltaTime();
		if (gameTimer_ <= 1.0f) {
			bigNumberUI_->SetTexture("BigNumber1.png");
			bigNumberUI_->SetRotate(-slopeAngle_ * toRadian);
		} else if (gameTimer_ <= 2.0f) {
			bigNumberUI_->SetTexture("BigNumber2.png");
			bigNumberUI_->SetRotate(slopeAngle_ * toRadian);
		} else if (gameTimer_ <= 3.0f) {
			bigNumberUI_->SetTexture("BigNumber3.png");
			bigNumberUI_->SetRotate(-slopeAngle_ * toRadian);
		} else if (gameTimer_ <= 4.0f) {
			bigNumberUI_->SetTexture("BigNumber4.png");
			bigNumberUI_->SetRotate(slopeAngle_ * toRadian);
		} else if (gameTimer_ <= 5.0f) {
			bigNumberUI_->SetTexture("BigNumber5.png");
			bigNumberUI_->SetRotate(-slopeAngle_ * toRadian);
		}

		BigNumberScalUp();

		bigNumberUI_->Update();
	} else {
		scaleUpTime_ = 0.0f;
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

	if (gameTimer_ <= 0.0f) {
		// Playerが飛んでいたら
		if (!isPlayerFlying) {
			// 制限時間を超えたら

			isFinish_ = true;
			//AudioPlayer::SinglShotPlay("timeUp.wav", 0.6f);

		} else { // オーバータイムに入る
			isOverTime_ = true;

			// 今飛んでいなくて前飛んでいたら
			if (!isPlayerFlying && isPreFlying_) {
				isFinish_ = true;
				//AudioPlayer::SinglShotPlay("timeUp.wav", 0.6f);
			}
		}
	}

	if (isFinish_) {
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
	if (Input::IsTriggerKey(DIK_0)) {
		gameTimer_ = 3;
	}
#endif // _DEBUG

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GamePlayTimer::Draw() const {
	timeGauge_->Draw();
	timeGaugeOutSide_->Draw();

	if (!isOverTime_ && !isFinish_) {
		if (gameTimer_ <= 5.0f) {
			bigNumberUI_->Draw();
		}
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
// ↓　TimeUpの文字のscaleを大きくする
//////////////////////////////////////////////////////////////////////////////////////////////////

void GamePlayTimer::BigNumberScalUp() {
	if (scaleUpTime_ <= scaleUpTimeLimit_) {
		float t = scaleUpTime_ / scaleUpTimeLimit_;
		bigNumberScale_ = Vector2::Lerp({ 0.0f, 0.0f }, { 1.0f, 1.0f }, EaseOutExpo(t));
		bigNumberUI_->SetScale(bigNumberScale_);
		bigNumberAlpha_ = 0.5f;

	} else {
		float t = (scaleUpTime_ - scaleUpTimeLimit_) / (1.0f - scaleUpTimeLimit_);
		bigNumberAlpha_ = std::lerp(0.5f, 0.0f, EaseInOutCubic(t));
	}

	if (scaleUpTime_ >= 1.0f) {
		scaleUpTime_ = 0.0f;
	}
	
	bigNumberUI_->SetColor(Vector4(1.0f, 1.0f, 1.0f, bigNumberAlpha_));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　調整項目の時間
//////////////////////////////////////////////////////////////////////////////////////////////////

void GamePlayTimer::AdaptAdjustmentItem() {
	originPos_ = adjustItem_->GetValue<Vector2>(groupName_, "numberOriginPos");
	timeGaugeBarOffset_ = adjustItem_->GetValue<Vector2>(groupName_, "timeGaugeBarOffset");
	addTimeOffsetX_ = adjustItem_->GetValue<float>(groupName_, "addTimeOffsetX");
	addTimeStartPos_ = adjustItem_->GetValue<Vector2>(groupName_, "addTimeStartPos");
	timeGaugeOutSide_->SetCenterPos(adjustItem_->GetValue<Vector2>(groupName_, "timeGaugeOutSidePos"));
	//timeGauge_->SetTextureCenterPos(adjustItem_->GetValue<Vector2>(groupName_, "clock_position"));
	//numberInterval_ = adjustItem_->GetValue<float>(groupName_, "numberInterval");
	//numberSpriteScale_ = adjustItem_->GetValue<Vector2>(groupName_, "numberSpriteScale");
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　残り時間通知のTextureの追加
//////////////////////////////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　時間の追加
//////////////////////////////////////////////////////////////////////////////////////////////////

void GamePlayTimer::AddTime(float time) {
	if (isOverTime_) {
		return;
	}

	if (time == 5.0f) {
		addTimeSprite_->SetTexture("missionClearAddTime_5.png");
	} else {
		addTimeSprite_->SetTexture("missionClearAddTime.png");
	}
		
	gameTimer_ += time;

	if (gameTimer_ > gameTimeLimit_) {
		gameTimeLimit_ = gameTimer_;
	}

	isAddTime_ = true;

	isAppearance_ = true;
	addClockMoveCount_ = 0.0f;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　時間追加の際の時計の動き
//////////////////////////////////////////////////////////////////////////////////////////////////

void GamePlayTimer::AddClockMove() {
	addClockMoveCount_ += GameTimer::DeltaTime();
	float t = addClockMoveCount_ / addClockMoveTime_;

	if (isAppearance_) {
		Vector2 pos = addTimeSprite_->GetCenterPos();
		pos = Vector2::Lerp(addTimeStartPos_, addTimeEndPos_, EaseOutExpo(t));
		addTimeSprite_->SetCenterPos(pos);

		alpa_ = std::lerp(0.0f, 1.0f, EaseOutExpo(t));
	} else {
		Vector2 pos = addTimeSprite_->GetCenterPos();
		pos = Vector2::Lerp(addTimeEndPos_, addTimeStartPos_, EaseInQuart(t));
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
// ↓　編集
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
void GamePlayTimer::Debug_Gui() {
	if (ImGui::TreeNode("GamePlayTimer")) {
		ImGui::Text("gameTimer_ : %f", gameTimer_);
		ImGui::SliderFloat("gameTimer", &gameTimer_, 0.0f, gameTimeLimit_);
		ImGui::DragFloat3("originPos", &originPos_.x, 0.1f);
		ImGui::DragFloat2("timeGaugeBarOffset", &timeGaugeBarOffset_.x, 0.1f);
		ImGui::DragFloat2("addTimeStartPos", &addTimeStartPos_.x, 0.1f);
		ImGui::DragFloat("addTimeOffsetX", &addTimeOffsetX_, 0.1f);
		/*ImGui::DragFloat2("numberSpriteScale", &numberSpriteScale_.x, 0.1f);
		ImGui::DragFloat("inverval", &numberInterval_, 0.1f);
		*/
		if (ImGui::TreeNode("timeGauge")) {
			timeGauge_->Debug_Gui();
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("timeGaugeOutSide")) {
			timeGaugeOutSide_->Debug_Gui();
			ImGui::TreePop();
		}

		if (ImGui::Button("AddTime")) {
			AddTime(10.0f);
		}

		if (ImGui::Button("ReAdapt")) {
			AdaptAdjustmentItem();
		}

		addTimeEndPos_ = timeGaugeOutSide_->GetCenterPos();
		addTimeEndPos_.x += addTimeOffsetX_;

		ImGui::TreePop();
	}
}
#endif


//int GamePlayTimer::DegitCount(float value) {
//	if (value == 0.0f) { return 0; }
//	// 桁数 = log10(値) + 1(std::log10はvalueが10以下の時は0が返される)
//	return static_cast<int>(std::floor(std::log10(value)) + 1);
//}
//
//int GamePlayTimer::IntegerCount(int value, int n) {
//	// 整数部の桁数を計算
//	/*int numDigits = static_cast<int>(std::log10(value)) + 1;
//	if (numDigits - n < 0) {
//		return 0;
//	}*/
//	// n桁目を取得するために数値を10で割ったあまりから割り出す
//	int num = (value / static_cast<int>(std::pow(10, n - 1)));
//	int result = num % 10;
//	return result;
//}
//
//Vector2 GamePlayTimer::CalculationSpriteLT(int value) {
//	if (value == 0) {
//		return { 0.0f, 0.0f };
//	} else if (value == 1) {
//		return { numberSpriteSize_.x , 0.0f };
//	} else if (value == 2) {
//		return { numberSpriteSize_.x * 2.0f, 0.0f };
//	} else if (value == 3) {
//		return { numberSpriteSize_.x * 3.0f, 0.0f };
//	} else if (value == 4) {
//		return { numberSpriteSize_.x * 4.0f, 0.0f };
//	} else if (value == 5) {
//		return { numberSpriteSize_.x * 5.0f, 0.0f };
//	} else if (value == 6) {
//		return { numberSpriteSize_.x * 6.0f, 0.0f };
//	} else if (value == 7) {
//		return { numberSpriteSize_.x * 7.0f, 0.0f };
//	} else if (value == 8) {
//		return { numberSpriteSize_.x * 8.0f , 0.0f };
//	} else {
//		return { numberSpriteSize_.x * 9.0f , 0.0f };
//	}
//}