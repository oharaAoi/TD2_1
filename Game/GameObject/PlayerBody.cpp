#include "PlayerBody.h"

PlayerBody::PlayerBody(){}

PlayerBody::~PlayerBody(){}

void PlayerBody::Update(){
	if(pTargetObject_){

		// ターゲットとの座標の差分
		Vector3 difVec{};
		difVec = transform_->GetTranslation() - pTargetObject_->GetTransform()->GetTranslation();

		// 差分が0なら直接値を設定
		if(Length(difVec) == 0.0f){
			difVec = { -1.0f,0.0f,0.0f };
		}

		// 追従
		transform_->SetTranslaion(pTargetObject_->GetTransform()->GetTranslation() + difVec.Normalize() * spaceLength_);

		// 角度を補間
		//float difAngle = transform_->GetQuaternion().z - pTargetObject_->GetTransform()->GetQuaternion().z;
		Quaternion slerp = Quaternion::Slerp(
			pTargetObject_->GetTransform()->GetQuaternion().Normalize(),
			transform_->GetQuaternion().Normalize(),
			0.8f).Normalize();
		transform_->SetQuaternion(slerp);
	}

	BaseGameObject::Update();
}
