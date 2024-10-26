#include "MissionUI.h"

MissionUI::MissionUI() {
}

MissionUI::~MissionUI() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void MissionUI::Init() {
	missionSpriteList_.push_back(Engine::CreateSprite("sky.png"));
	missionSpriteList_.push_back(Engine::CreateSprite("sky.png"));
	missionSpriteList_.push_back(Engine::CreateSprite("sky.png"));

	for (auto& mission : missionSpriteList_) {
		mission->SetRectRange({ 200.0f, 150.0f });
		mission->SetTextureSize({ 200.0f, 150.0f });
	}

	topStartPos_ = { 2000.0f, 350.0f };
	topEndPos_ = { 1000.0f, 350.0f };

	interval_ = 100.0f;

	moveTimeCount_ = 0.0f;
	moveTime_ = 1.0f;

	isAppearance_ = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void MissionUI::Update() {
	for (auto& mission : missionSpriteList_) {
		mission->Update();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void MissionUI::Draw() const {
	for (const auto& mission : missionSpriteList_) {
		mission->Draw();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　登場処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void MissionUI::MissionAppearance() {
	moveTimeCount_ += GameTimer::DeltaTime();
	float t = moveTimeCount_ / moveTime_;

	for (uint32_t index = 0; index < missionSpriteList_.size() - 1; ++index) {
		Vector2 pos = missionSpriteList_[index]->GetCenterPos();
		Vector2 startPos = { topStartPos_.x, topStartPos_.y + interval_ };
		Vector2 endPos = { topEndPos_.x, topEndPos_.y + interval_ };
		
		// lerpさせる
		pos = Vector2::Lerp(startPos, endPos, t);

		missionSpriteList_[index]->SetCenterPos(pos);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Fade処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void MissionUI::MissionDisappear() {

}

#ifdef _DEBUG
void MissionUI::Debug_Gui() {
	if (ImGui::TreeNode("MissionUI")) {
		ImGui::DragFloat3("offsetTopPos", &offsetTopPos_.x, 1.0f);
		ImGui::TreePop();
	}
}
#endif // _DEBUG