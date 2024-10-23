#include "PlayerControlUI.h"

PlayerControlUI::PlayerControlUI() {
}

PlayerControlUI::~PlayerControlUI() {
}

void PlayerControlUI::Init() {
	offset_ = { -130.0f, -60.0f };
	scale_ = { 0.5f, 0.5f };

	sprite_ui_ = Engine::CreateSprite("UI_fly1.png");
	sprite_ui_->SetScale(scale_);
}

void PlayerControlUI::Update(const Vector2& pos, bool isGliding) {
	if (isGliding) {
		// 滑空していたら
		sprite_ui_->SetTexture("UI_fly2.png");
	} else {
		// 落下状態だったら
		sprite_ui_->SetTexture("UI_fly1.png");
	}

	sprite_ui_->SetCenterPos(pos + offset_);
	sprite_ui_->SetScale(scale_);

	sprite_ui_->Update();
}

void PlayerControlUI::Draw() const {
	sprite_ui_->Draw();
}

#ifdef _DEBUG
void PlayerControlUI::Debug_Gui() {
	if (ImGui::TreeNode("PlayerControl")) {
		ImGui::DragFloat3("offset", &offset_.x, 1.0f);
		ImGui::DragFloat3("scale", &scale_.x, 1.0f);
		sprite_ui_->Debug_Gui();
		ImGui::TreePop();
	}
}
#endif // _DEBUG