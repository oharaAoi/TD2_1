#include "PlayerAnimator.h"

PlayerAnimator::PlayerAnimator() {}
PlayerAnimator::~PlayerAnimator() {}

void PlayerAnimator::Init(Player* pPlayer) {
	pPlayer_ = pPlayer;
	playerHiSpeedEffect_ = std::make_unique<BaseGameObject>();
	playerHiSpeedEffect_->Init();
	playerHiSpeedEffect_->SetObject("HighSpeedEffect.gltf");
	playerHiSpeedEffect_->SetAnimater("./Game/Resources/Model/Effect/","HighSpeedEffect.gltf", true);
	playerHiSpeedEffect_->SetIsLighting(false);
}

void PlayerAnimator::Update() {
	if (pPlayer_->GetMoveSpeed() > 30.0f) {
		playerHiSpeedEffect_->GetTransform()->SetTranslaion(pPlayer_->GetWorldTranslation());
		playerHiSpeedEffect_->GetTransform()->SetQuaternion(pPlayer_->GetTransform()->GetQuaternion());
		playerHiSpeedEffect_->Update();
	}
}

void PlayerAnimator::Draw() const {
	if (pPlayer_->GetMoveSpeed() > 30.0f) {
		playerHiSpeedEffect_->Draw();
	}
}
