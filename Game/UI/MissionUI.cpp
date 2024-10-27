#include "MissionUI.h"

MissionUI::MissionUI() {
}

MissionUI::~MissionUI() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void MissionUI::Init(GamePlayTimer* pGamePlayTimer) {
	pGamePlayTimer_ = pGamePlayTimer;

	topStartPos_ = { -400.0f, 200.0f };
	topEndPos_ = { 150.0f, 200.0f };

	interval_ = 100.0f;

	// 登場系
	appearanceSpeedCount_ = 0.0f;
	appearanceHeightCount_ = 0.0f;
	appearanceTime_ = 1.0f;

	isSpeesAppearanceFinish_  = false;
	isHeightAppearanceFinish_ = false;

	// 消える系
	disappearSpeedCount_ = 0;
	disappearHeightCount_ = 0;
	disappearTime_ = 1.0f;

	// check系
	speedCheckCount_ = 0.0f;
	heightCheckCount_ = 0.0f;
	checkAppearanceTime_ = 1.0f;

	speedCheckWaitTime_ = 0.0f;
	heightCheckWaitTime_ = 0.0f;
	waitTime_ = 1.0f;

	isSpeedCheck_ = false;
	isHeightCheck_ = false;

	nowSpeedMission_ = SpeedMission::Mission_100;
	nowHeightMission_ = HeightMission::Mission_300;

	// -------------------------------------------------
	// ↓ Spriteの作成、初期化
	// -------------------------------------------------
	speedCheck_ = Engine::CreateSprite("missionClear.png");
	heightCheck_ = Engine::CreateSprite("missionClear.png");

	speedMission_ = Engine::CreateSprite("mission2_speed.png");
	heightMission_ = Engine::CreateSprite("mission1_hight.png");

	speedMission_->SetScale({ 0.6f, 0.6f });
	heightMission_->SetScale({ 0.6f, 0.6f });

	speedMission_->SetCenterPos(topStartPos_);
	heightMission_->SetCenterPos({ topStartPos_.x, topStartPos_.y + interval_ });

	speedCheck_->SetCenterPos({ -300.0f, 0.0f });
	heightCheck_->SetCenterPos({ -300.0f, 0.0f });
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void MissionUI::Update(float playerSpeed, float playerPosY) {
	// -------------------------------------------------
	// ↓ 両方完了していたら
	// -------------------------------------------------
	if (nowSpeedMission_ == SpeedMission::Mission_Finish && nowHeightMission_ == HeightMission::Mission_Finish) {
		if (!isHeightAppearance_) {
			isHeightAppearance_ = true;
			heightMission_->SetTexture("missionComplete.png");
		}
	}

	// -------------------------------------------------
	// ↓ 出現に関する処理
	// -------------------------------------------------
	if (isSpeesAppearanceFinish_) {
		if (playerSpeed >= (float)nowSpeedMission_) {
			// タイムを増やす
			if (!isSpeedCheck_) {
				pGamePlayTimer_->AddTime(10.0f);
			}
			isSpeedCheck_ = true;
		}
	}

	if (isHeightAppearanceFinish_) {
		if (playerPosY >= (float)nowHeightMission_) {
			if (!isHeightCheck_) {
				pGamePlayTimer_->AddTime(10.0f);
			}
			isHeightCheck_ = true;
		}
	}

	// -------------------------------------------------
	// ↓ 出現に関する処理
	// -------------------------------------------------
	if (isSpeesAppearance_) {
		SpeedMissionAppearance();
	}

	if (isHeightAppearance_) {
		HeightMissionAppearance();
	}

	// -------------------------------------------------
	// ↓ 消える処理に関する
	// -------------------------------------------------
	// 消えるまでの待機時間以下だったら
	// 速度のミッション
	if (!isSpeesAppearance_) {
		if (speedCheckWaitTime_ < waitTime_) {
			if (isSpeedCheck_) {
				SpeedCheck();
			}
		} else {
			SpeedMissionDisappear();
		}
	}

	// 高さのミッション
	if (!isHeightAppearance_) {
		if (heightCheckWaitTime_ < waitTime_) {
			if (isHeightCheck_) {
				HeightCheck();
			}
		} else {
			if (!(nowHeightMission_ == HeightMission::Mission_Finish)) {
				HeightMissionDisappear();
			}
		}
	}

	// -------------------------------------------------
	// ↓ 更新
	// -------------------------------------------------
	speedCheck_->Update();
	heightCheck_->Update();

	speedMission_->Update();
	heightMission_->Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void MissionUI::Draw() const {
	speedCheck_->Draw();
	heightCheck_->Draw();

	speedMission_->Draw();
	heightMission_->Draw();
}

void MissionUI::Start() {
	isHeightAppearance_ = true;
	isSpeesAppearance_ = true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　登場処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void MissionUI::SpeedMissionAppearance() {
	if (isSpeesAppearanceFinish_) {
		return;
	}

	appearanceSpeedCount_ += GameTimer::DeltaTime();
	float t = appearanceSpeedCount_ / appearanceTime_;

	Vector2 speedPos = Vector2::Lerp(topStartPos_, topEndPos_, EaseOutBack(t));
	speedMission_->SetCenterPos(speedPos);

	if (appearanceSpeedCount_ > appearanceTime_) {
		isSpeesAppearance_ = false;
		isSpeesAppearanceFinish_ = true;
		appearanceSpeedCount_ = 0.0f;
	}
}

void MissionUI::HeightMissionAppearance() {
	if (isHeightAppearanceFinish_) {
		return;
	}

	appearanceHeightCount_ += GameTimer::DeltaTime();
	float t = appearanceHeightCount_ / appearanceTime_;

	Vector2 heightPos = Vector2::Lerp(
		{ topStartPos_.x, topStartPos_.y + interval_ },
		{ topEndPos_.x, topEndPos_.y + interval_ },
		EaseOutBack(t)
	);

	heightMission_->SetCenterPos(heightPos);

	if (appearanceHeightCount_ > appearanceTime_) {
		if (!(nowHeightMission_ == HeightMission::Mission_Finish)) {
			isHeightAppearance_ = false;
		}

		isHeightAppearanceFinish_ = true;
		appearanceHeightCount_ = 0.0f;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Fade処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void MissionUI::SpeedMissionDisappear() {
	// 時間を過ぎていたら
	if (disappearSpeedCount_ > disappearTime_) {
		SpeedMissionChange();
		disappearSpeedCount_ = 0.0f;
		return;
	}

	// 時間の計測
	disappearSpeedCount_ += GameTimer::DeltaTime();
	float t = disappearSpeedCount_ / disappearTime_;

	// 座標の移動
	Vector2 speedPos = Vector2::Lerp(topEndPos_, topStartPos_, EaseInBack(t));
	speedMission_->SetCenterPos(speedPos);

	// ついでにCheckマークも消す
	Vector2 scale;
	scale = Vector2::Lerp({ 0.5f, 0.5f }, { 0.0f, 0.0f }, EaseOutElastic(t));
	speedCheck_->SetScale(scale);
}

void MissionUI::HeightMissionDisappear() {
	
	if (disappearHeightCount_ > disappearTime_) {
		HeightMissionChange();
		disappearHeightCount_ = 0.0f;
		return;
	}

	// 時間の計測
	disappearHeightCount_ += GameTimer::DeltaTime();
	float t = disappearHeightCount_ / disappearTime_;

	// 座標の移動
	Vector2 heightPos = Vector2::Lerp(
		{ topEndPos_.x, topEndPos_.y + interval_ },
		{ topStartPos_.x, topStartPos_.y + interval_ },
		EaseInBack(t)
	);

	heightMission_->SetCenterPos(heightPos);

	// ついでにCheckマークも消す
	Vector2 scale = heightCheck_->GetScale();
	scale = Vector2::Lerp({ 0.5f, 0.5f }, { 0.0, 0.0f }, EaseOutElastic(t));
	heightCheck_->SetScale(scale);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Checkをつかる処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void MissionUI::SpeedCheck() {
	if (speedCheckCount_ > checkAppearanceTime_) {
		speedCheckWaitTime_ += GameTimer::DeltaTime();
		return;
	}

	// countを行う
	speedCheckCount_ += GameTimer::DeltaTime();
	float t = speedCheckCount_ / checkAppearanceTime_;

	// 座標を指定する
	Vector2 pos = speedMission_->GetCenterPos();
	pos.x += 150.0f;
	pos.y += 10.0f;
	speedCheck_->SetCenterPos(pos);

	// 大きさを変更する
	Vector2 scale;
	scale = Vector2::Lerp({ 1.0f, 1.0f }, { 0.5, 0.5f }, EaseOutElastic(t));
	speedCheck_->SetScale(scale);
}

void MissionUI::HeightCheck() {
	if (heightCheckCount_ > checkAppearanceTime_) {
		heightCheckWaitTime_ += GameTimer::DeltaTime();
		return;
	}

	// countを行う
	heightCheckCount_ += GameTimer::DeltaTime();
	float t = heightCheckCount_ / checkAppearanceTime_;

	// 座標を指定する
	Vector2 pos = heightMission_->GetCenterPos();
	pos.x += 150.0f;
	pos.y += 10.0f;
	heightCheck_->SetCenterPos(pos);

	// 大きさを変更する
	Vector2 scale;
	scale = Vector2::Lerp({ 1.0f, 1.0f }, { 0.5f, 0.5f }, EaseOutElastic(t));
	heightCheck_->SetScale(scale);
}

void MissionUI::SpeedMissionChange() {
	// checkのwaitとcountも初期化する
	speedCheckCount_ = 0.0f;
	speedCheckWaitTime_ = 0.0f;

	isSpeesAppearance_ = true;
	isSpeedCheck_ = false;
	isSpeesAppearanceFinish_ = false;

	// textureを変える
	switch (nowSpeedMission_) {
	case SpeedMission::Mission_100:
		nowSpeedMission_ = SpeedMission::Mission_150;
		break;
	case SpeedMission::Mission_150:
		nowSpeedMission_ = SpeedMission::Mission_Finish;
		isSpeesAppearance_ = false;
		break;
	}
}

void MissionUI::HeightMissionChange() {
	heightCheckCount_ = 0.0f;
	heightCheckWaitTime_ = 0.0f;

	isHeightAppearance_ = true;
	isHeightCheck_ = false;
	isHeightAppearanceFinish_ = false;

	// textureを変える
	switch (nowHeightMission_) {
	case HeightMission::Mission_300:
		nowHeightMission_ = HeightMission::Mission_500;
		break;
	case HeightMission::Mission_500:
		nowHeightMission_ = HeightMission::Mission_Finish;
		heightMission_->SetScale({0.5f, 0.5f});
		isHeightAppearance_ = false;
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void MissionUI::Debug_Gui() {
	if (ImGui::TreeNode("MissionUI")) {
		ImGui::DragFloat3("offsetTopPos", &offsetTopPos_.x, 1.0f);
		ImGui::DragFloat3("topStartPos", &topStartPos_.x, 1.0f);
		ImGui::DragFloat3("topEndPos", &topEndPos_.x, 1.0f);

		if (ImGui::Button("ReAppearance")) {
			SpeedMissionChange();
			HeightMissionChange();
		}

		if (ImGui::Button("speedCheck")) {
			isSpeedCheck_ = true;
			speedCheckCount_ = 0.0f;
			speedCheckWaitTime_ = 0.0f;
		}

		if (ImGui::Button("heightCheck")) {
			isHeightCheck_ = true;
			heightCheckCount_ = 0.0f;
			heightCheckWaitTime_ = 0.0f;
		}


		ImGui::TreePop();
	}
}
#endif // _DEBUG