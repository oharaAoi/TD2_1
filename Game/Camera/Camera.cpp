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
		// ターゲットからの距離を水深に応じて更新
		offsetLength_ = 80.0f + pPlayer_->GetSwimmingDepth() * 3.0f;

	} else{
		target_ = pPlayer_->GetTransform();
		offsetLength_ = 80.0f;
	}

	// -------------------------------------------------
	// ↓ Targetがいたら
	// -------------------------------------------------
	if(target_ != nullptr) {

		Vector3 dif{};
		Vector3 addVec{};

		if(pPlayer_->GetIsFlying()){

			if(pPlayer_->GetTransform()->GetTranslation().y > 10.0f){

 				float heightRatio = std::clamp((pPlayer_->GetTransform()->GetTranslation().y - 10.0f) / 40.0f, 0.0f, 1.0f);
				
				// translate
				addVec.y = 7.0f * (pPlayer_->GetTransform()->GetTranslation().y - 10.0f) / 40.0f;
				addVec.x = 10.0f * heightRatio;

				// rotate
				transform_.rotate.x += 0.005f * GameTimer::TimeRate();
				transform_.rotate.x = std::clamp(transform_.rotate.x, baseRotate.x, baseRotate.x + 0.15f);
				transform_.rotate.y += 0.005f * GameTimer::TimeRate();
				transform_.rotate.y = std::clamp(transform_.rotate.y, baseRotate.y, baseRotate.y + 0.15f);
			
			}

		} else{
			transform_.rotate.x -= 0.005f * GameTimer::TimeRate();
			transform_.rotate.x = std::clamp(transform_.rotate.x, baseRotate.x, baseRotate.x + 0.15f);
			transform_.rotate.y -= 0.005f * GameTimer::TimeRate();
			transform_.rotate.y = std::clamp(transform_.rotate.y, baseRotate.y, baseRotate.y + 0.15f);
		}

		// 目的座標への差分
		dif = (pPlayer_->GetTransform()->GetTranslation() + offsetVec_ * offsetLength_ + addVec) - transform_.translate;

		// 少し遅らせて追従
		transform_.translate += dif * 0.05f * GameTimer::TimeRate();
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
