#include "Camera.h"

Camera::Camera(){
	Init();
}

Camera::~Camera(){}

void Camera::Finalize(){}

void Camera::Init(){
	BaseCamera::Init();

	transform_ = {
		{1.0f, 1.0f, 1.0f},
		{0.0f, 0.91f, 0.0f},
		{0.0f, 0.0f, 0.0f}
	};

	offsetVec_ = Vector3(-20.4f, 0.3f, -25.7f).Normalize();
}

void Camera::Update(){

	//プレイヤーが水面に出ているかどうかでターゲットを変更
	if(pPlayer_->GetTransform()->GetTranslation().y < 10.0f){
		target_ = pPlayer_->GetAboveSurfaceTransform();
		offsetLength_ = 40.0f + pPlayer_->GetSwimmingDepth() * 5.0f;
		transform_.rotate.y = 0.91f + (-0.5f) * std::clamp(pPlayer_->GetSwimmingDepth() / 30.0f,0.0f,1.0f);
		

	} else{
		target_ = pPlayer_->GetTransform();
		offsetLength_ = 40.0f;
	}

	// -------------------------------------------------
	// ↓ Targetがいたら
	// -------------------------------------------------
	if(target_ != nullptr) {

		// 目的座標への差分
		Vector3 dif = (target_->GetTranslation() + 
			(offsetVec_ * offsetLength_ * MakeRotateYMatrix(0.5f * std::clamp(pPlayer_->GetSwimmingDepth() / 30.0f, 0.0f, 1.0f))))
			- transform_.translate;

		// 少し遅らせて追従
		transform_.translate += dif * 0.1f;
	}

	BaseCamera::Update();
}

#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
void Camera::Debug_Gui(){
	ImGui::Begin("Camera");
	if(ImGui::Button("Reset")) {
		transform_ = { {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -9.0f} };
		transform_.rotate = { 0.0f, 0.0f, 0.0f };
		rotateMat_ = MakeRotateXYZMatrix(transform_.rotate);
	}

	ImGui::DragFloat3("translate", &transform_.translate.x, 0.1f);
	ImGui::DragFloat3("rotate", &transform_.rotate.x, 0.1f);

	ImGui::End();
}
#endif
