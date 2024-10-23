#include "PlayerAnimator.h"

PlayerAnimator::PlayerAnimator(){}
PlayerAnimator::~PlayerAnimator(){}

void PlayerAnimator::Init(Player* pPlayer){
	pPlayer_ = pPlayer;
	playerHiSpeedEffect_ = std::make_unique<BaseGameObject>();
	playerHiSpeedEffect_->Init();
	playerHiSpeedEffect_->SetObject("HighSpeedEffect.gltf");
	playerHiSpeedEffect_->SetAnimater("./Game/Resources/Model/Effect/", "HighSpeedEffect.gltf", true);
	playerHiSpeedEffect_->SetIsLighting(false);
}

void PlayerAnimator::Update(){
	if(pPlayer_->GetMoveSpeed() > drawSpeed) {

		if(drawSpeed - pPlayer_->GetMoveSpeed() != 0){
			easeT = (drawSpeed - pPlayer_->GetMoveSpeed()) / (drawSpeed - pPlayer_->GetMaxMoveSpeed());
		} else{
			easeT = 0;
		}
		easeT = std::clamp(easeT, 0.0f, 1.0f);
		Vector3 newScale = Vector3::Lerp(sizeMin, sizeMax, easeT);
		playerHiSpeedEffect_->GetTransform()->SetScale(newScale);
		playerHiSpeedEffect_->GetTransform()->SetTranslaion(pPlayer_->GetWorldTranslation());
		playerHiSpeedEffect_->GetTransform()->SetQuaternion(pPlayer_->GetTransform()->GetQuaternion());
		playerHiSpeedEffect_->Update();

		playerHiSpeedEffect_->SetColor(color_);
	}
}

void PlayerAnimator::Draw() const{
	if(pPlayer_->GetMoveSpeed() > drawSpeed) {
		playerHiSpeedEffect_->Draw();
	}
}

void PlayerAnimator::SetColor(int birdJumpNum) {
	if (birdJumpNum == 0) {
		color_ = { 1.0f , 1.0f, 1.0f, 1.0f };
	} else if(birdJumpNum == 1) {
		color_ = { 1.0f , 0.5f, 0.0f, 1.0f };
	} else if (birdJumpNum == 2) {
		color_ = { 1.0f , 1.0f, 0.0f, 1.0f };
	}
}
