#include "FadePanel.h"
#include "Engine/Engine.h"

FadePanel::FadePanel() {}
FadePanel::~FadePanel() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void FadePanel::Init() {
	time_ = 0.0f;
	timeLimit_ = 1.0f;
	panelAlpha_ = 0.0f;

	doNothing_ = true;
	isBlackOut_ = false;
	isFinished_ = false;

	panel_ = Engine::CreateSprite("panel.png");
	panel_->SetCenterPos(Vector2(640.0f, 360.0f));
	panel_->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void FadePanel::Update() {
	if (doNothing_) {
		return;
	}

	if (time_ >= timeLimit_) {
		isFinished_ = true;
		doNothing_ = true;
		return;
	}

	// -------------------------------------------------
	// ↓ 実際にカラーを調整する
	// -------------------------------------------------
	if (isBlackOut_) {	// ブラックアウトさせる
		time_ += GameTimer::DeltaTime();
		time_ = std::clamp(time_, 0.0f, timeLimit_);

		float t = time_ / timeLimit_;
		panelAlpha_ = std::lerp(0.0f, 1.0f, t);

		panel_->SetColor(Vector4(1.0f, 1.0f, 1.0f, panelAlpha_));

	} else {			// ブラックアウトから抜ける
		time_ += GameTimer::DeltaTime();
		time_ = std::clamp(time_, 0.0f, timeLimit_);

		float t = time_ / timeLimit_;
		panelAlpha_ = std::lerp(1.0f, 0.0f, t);

		panel_->SetColor(Vector4(1.0f, 1.0f, 1.0f, panelAlpha_));
	}

	panel_->Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
/////////////////////////////////////////////////////////////////////////////////////////////////

void FadePanel::Draw() const {
	panel_->Draw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　fadeOut処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void FadePanel::SetFadeOut(float timeLimit) {
	isBlackOut_ = true;
	doNothing_ = false;
	isFinished_ = false;
	time_ = 0.0f;
	timeLimit_ = timeLimit;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　fadeOutを開ける処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void FadePanel::SetFadeOutOpen(float timeLimit) {
	isBlackOut_ = false;
	doNothing_ = false;
	isFinished_ = false;
	time_ = 0.0f;
	timeLimit_ = timeLimit;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　編集処理
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void FadePanel::Debug_Gui() {
	ImGui::Begin("panel");
	ImGui::DragFloat("time", &time_, 0.1f);
	ImGui::DragFloat("timeLimit", &timeLimit_, 0.1f);
	ImGui::DragFloat("panelAlpha", &panelAlpha_, 0.1f);
	if (ImGui::Button("BlackOut")) {
		SetFadeOut();
	}
	ImGui::SameLine();
	if (ImGui::Button("BlackOutOpne")) {
		SetFadeOutOpen();
	}

	ImGui::End();
}
#endif