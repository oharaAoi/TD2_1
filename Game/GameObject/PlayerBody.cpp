#include "PlayerBody.h"
#include "Engine/Math/Easing.h"

PlayerBody::PlayerBody(){
	scaleUpTimeCount_ = 0.0f;
	scaleDownTimeCount_ = 0.0f;
	isScaleDown_ = false;
}

PlayerBody::~PlayerBody(){}

void PlayerBody::Update(){
	if (isNewCreate_) {
		ScaleUpBody();
	}

	if(pTargetObject_){

		// ターゲットとの座標の差分
		Vector3 difVec{};
		difVec = transform_->GetTranslation() - pTargetObject_->GetTransform()->GetTranslation();

		// 差分が0なら直接値を設定
		if(Length(difVec) == 0.0f){
			difVec = (Vector3(1.0f, 0.0f, 0.0f) * pTargetObject_->GetTransform()->GetQuaternion().MakeMatrix()) * -1.0f;
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

void PlayerBody::ScaleUpBody() {
	// scaleを標準のサイズに戻す
	if (isScaleDown_) {
		// scaleを大きくする
		scaleDownTimeCount_ += GameTimer::DeltaTime();
		scaleDownTimeCount_ = std::clamp(scaleUpTimeCount_, 0.0f, scaleUpTime_);

		float t = scaleDownTimeCount_ / scaleDownTime_;
		Vector3 lerpScale = Vector3::Lerp({ 0,0,0 }, upScale_, EaseOutElastic(t));
		transform_->SetScale(lerpScale);

		if (scaleDownTimeCount_ >= scaleDownTime_) {
			isScaleDown_ = false;
		}

	} else {
		// scaleを大きくする
		scaleUpTimeCount_ += GameTimer::DeltaTime();
		scaleUpTimeCount_ = std::clamp(scaleUpTimeCount_, 0.0f, scaleUpTime_);

		float t = scaleUpTimeCount_ / scaleUpTime_;
		Vector3 lerpScale = Vector3::Lerp(upScale_, { 1,1,1 }, EaseOutBounce(t));
		transform_->SetScale(lerpScale);
	}
	

	if (scaleUpTimeCount_ >= scaleUpTime_) {
		isNewCreate_ = false;
	}
}

void PlayerBody::UpdateMatrix(){
	BaseGameObject::Update();
}

void PlayerBody::SetScaleUp() {
	scaleUpTimeCount_ = false;
	isNewCreate_ = true;
}
