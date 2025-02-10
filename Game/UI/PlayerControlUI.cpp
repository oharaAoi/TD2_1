#include "PlayerControlUI.h"
#include "Engine/Json/JsonAdjustmentItem.h"

PlayerControlUI::PlayerControlUI() {
}

PlayerControlUI::~PlayerControlUI() {
}

void PlayerControlUI::Init() {
	parameter_.FromJson(JsonAdjustmentItem::GetData(groupName_, ItemName_));

	spaceButton_ = Engine::CreateSprite("SpaceButton.png");
	spaceButton_->SetScale({0.3f, 0.3f});
}

void PlayerControlUI::Update(const Vector2& playerScreenPos) {
	if (Input::IsPressKey(DIK_SPACE)) {
		spaceButton_->SetColor(Vector4(0.3f, 0.3f, 0.3f, 1.0f ));
	} else {
		spaceButton_->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	}
	spaceButton_->SetCenterPos(playerScreenPos + parameter_.spaceOffset);

	spaceButton_->Update();
}

void PlayerControlUI::Draw(bool isPlayerFlying) const {
	if (isPlayerFlying) {
		//sprite_ui_->Draw();
	}

	if (!pPlayer_->GetIsEnableLaunch() && !pPlayer_->GetIsFacedBird() && !pPlayer_->GetIsCutIn()) {
		spaceButton_->Draw();
	}
}

#ifdef _DEBUG
void PlayerControlUI::Debug_Gui() {
	if (ImGui::TreeNode("PlayerControl")) {
		ImGui::BulletText("spaceButton");
		ImGui::DragFloat2("spaceButtonOffset", &parameter_.spaceOffset.x, 2.0f);

		if (ImGui::TreeNode("SpaceButton")) {
			spaceButton_->Debug_Gui();
			ImGui::TreePop();
		}

		if (ImGui::Button("Save")) {
			JsonAdjustmentItem::Save(groupName_, parameter_.ToJson(ItemName_));
		}

		if (ImGui::Button("Apply")) {
			parameter_.FromJson(JsonAdjustmentItem::GetData(groupName_, ItemName_));
		}
		
		ImGui::TreePop();
	}
}
#endif // _DEBUG