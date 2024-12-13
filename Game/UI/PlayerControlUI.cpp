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

	spaceButton_ = Engine::CreateSprite("SpaceButton.png");
	spaceButton_->SetScale({0.4f, 0.4f});
	spaceButton_->SetCenterPos({380.0f, 640.0f});
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

	if (Input::IsPressKey(DIK_SPACE)) {
		spaceButton_->SetColor(Vector4(0.3f, 0.3f, 0.3f, 1.0f ));
	} else {
		spaceButton_->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	spaceButton_->Update();
}

void PlayerControlUI::Draw(bool isPlayerFlying) const {
	if (isPlayerFlying) {
		sprite_ui_->Draw();
	}
	spaceButton_->Draw();
}

#ifdef _DEBUG
void PlayerControlUI::Debug_Gui() {
	if (ImGui::TreeNode("PlayerControl")) {
		ImGui::DragFloat3("offset", &offset_.x, 1.0f);
		ImGui::DragFloat3("scale", &scale_.x, 1.0f);
		if (ImGui::TreeNode("flyingOrFall")) {
			sprite_ui_->Debug_Gui();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("SpaceButton")) {
			spaceButton_->Debug_Gui();
			ImGui::TreePop();
		}
		
		ImGui::TreePop();
	}
}
#endif // _DEBUG