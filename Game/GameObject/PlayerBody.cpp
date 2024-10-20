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

		angle_ = std::atan2(-difVec.y, -difVec.x);


		// 角度を補間
		//float difAngle = transform_->GetQuaternion().z - pTargetObject_->GetTransform()->GetQuaternion().z;
		/*Quaternion slerp = Quaternion::Slerp(
			pTargetObject_->GetTransform()->GetQuaternion().Normalize(),
			transform_->GetQuaternion().Normalize(),
			0.8f).Normalize();*/
		newRotate = newRotate.AngleAxis(angle_, { 0,0,1 });
		//transform_->GetQuaternion().Normalize()* newRotate.Normalize();
		transform_->SetQuaternion(newRotate);
	}

	SetIsLighting(false);

	BaseGameObject::Update();
}

void PlayerBody::Debug(int num){
#ifdef _DEBUG
	std::string name = "torso" + std::to_string(num);
	ImGui::Begin(name.c_str());
	//ImGui::DragFloat("torsoAngle", &angle_, 0.1f);
	ImGui::DragFloat("angle", &angle_, 0.01f);
	ImGui::Text("newRotate=%f,%f,%f", newRotate.x, newRotate.y, newRotate.z);
	Debug_Gui();
	ImGui::End();
#endif // DEBUG}
}