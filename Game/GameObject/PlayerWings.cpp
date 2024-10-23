#include "PlayerWings.h"

PlayerWings::PlayerWings() {}
PlayerWings::~PlayerWings() {}

void PlayerWings::Init() {
	for (uint32_t oi = 0; oi < 2; ++oi) {
		wings_[oi] = std::make_unique<BaseGameObject>();
		wings_[oi]->Init();
		wings_[oi]->SetObject("Wing.gltf");
		wings_[oi]->SetAnimater("./Game/Resources/Model/AnimationWing/", "Wing.gltf", true);
		wings_[oi]->SetAnimationControlScrilpt(true);
		wings_[oi]->SetIsLighting(false);
		if (oi == 0) {
			wings_[oi]->GetTransform()->SetScale({ 1.0f, 1.0f, -1.0f });
		}

	}

	windOpenTime_ = 0;
	windOpenTimeLimit_ = 0.5f;
	rotateAxis_ = { 1.0f, 0.0f, 0.0f };
}

void PlayerWings::Update(const Vector3& parentPos, float time) {

	time_ += GameTimer::DeltaTime();
	time_ = std::fmod(time_, 1.0f);

	for (uint32_t oi = 0; oi < 2; ++oi) {
		// 右か左かを判別する
		float hugo = 1.0f;
		if (oi == 1) {
			hugo *= -1.0f;
		}
		// アニメーションのタイムを設定する
		wings_[oi]->GetAnimetor()->SetAnimationTime(time);
		// 羽根の座標を求める
		Vector3 pos = parentPos;
		pos.z += (offsetZ_ * hugo);
		wings_[oi]->GetTransform()->SetTranslaion(pos);
		// 更新処理
		wings_[oi]->GetAnimetor()->UpdateScript();
		wings_[oi]->Update();
	}
}

void PlayerWings::Draw() const {
	for (uint32_t oi = 0; oi < 2; ++oi) {
		wings_[oi]->Draw();
	}
}

void PlayerWings::NotFlying() {
	windOpenTime_ = 0;
	for (uint32_t oi = 0; oi < 2; ++oi) {
		wings_[oi]->GetTransform()->SetQuaternion(Quaternion());
	}
}

#ifdef _DEBUG
void PlayerWings::Debug_Gui() {
	if (ImGui::TreeNode("wings")) {
		ImGui::DragFloat("offsetZ", &offsetZ_, 0.1f);
		ImGui::DragFloat("openAngle", &openAngle_, 1.0f);
		ImGui::DragFloat("windOpenTimeLimit", &windOpenTimeLimit_, 0.1f);
		ImGui::DragFloat3("rotateAxis", &rotateAxis_.x, 0.1f);
		ImGui::SliderFloat("time", &windOpenTime_, 0.0f, windOpenTimeLimit_);
		for (uint32_t oi = 0; oi < 2; ++oi) {
			float hugo = 1;
			if (oi == 1) {
				hugo *= -1;
			}
			/*windOpenTime_ += GameTimer::DeltaTime();*/
			moveQuaternion_ = Quaternion::Slerp(
				Quaternion(),
				Quaternion::AngleAxis((openAngle_ * toRadian) * hugo, rotateAxis_),
				(windOpenTime_ / windOpenTimeLimit_)
			);
			wings_[oi]->GetTransform()->SetQuaternion(moveQuaternion_);
		}
		ImGui::TreePop();
	}
}
#endif