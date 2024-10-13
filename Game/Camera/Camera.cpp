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
		offsetLength_ = 60.0f + pPlayer_->GetSwimmingDepth() * 5.0f;
		transform_.rotate.y = 0.91f + (-0.5f) * std::clamp((pPlayer_->GetSwimmingDepth() - 10.0f) / 30.0f, 0.0f, 1.0f);


	} else{
		target_ = pPlayer_->GetTransform();
		offsetLength_ = 60.0f;
	}

	// -------------------------------------------------
	// ↓ Targetがいたら
	// -------------------------------------------------
	if(target_ != nullptr) {

		Vector3 dif{};

		if(pPlayer_->GetIsFlying()){
			offsetLength_ = 60.0f + std::fabsf((pPlayer_->GetSwimmingDepth())) * 0.5f;
			transform_.rotate.y = 0.91f + 0.05f * std::clamp(((pPlayer_->GetSwimmingDepth() - 10.0f) / -30.0f), 0.0f, 1.0f);
			transform_.rotate.x = 0.1f * std::clamp(((pPlayer_->GetSwimmingDepth() - 10.0f) / -30.0f), 0.0f, 1.0f);

		} else{
			transform_.rotate.x *= 0.005f * GameTimer::TimeRate();
			Vector3 offsetVecDif = Vector3(-20.4f, 0.3f, -25.7f).Normalize() - offsetVec_;
			offsetVec_ += offsetVecDif * 0.05f * GameTimer::TimeRate();
			float goalTranslateY = 10.0f * ((pPlayer_->GetSwimmingDepth() - 10.0f) / 30.0f);
			transform_.translate.y += (goalTranslateY - transform_.translate.y) * 0.5f * GameTimer::TimeRate();
			///transform_.translate.y = goalTranslateY;
		}

		// 目的座標への差分
		if(pPlayer_->GetIsFlying()){

			dif = (target_->GetTranslation() +
				((offsetVec_ * offsetLength_) *
					MakeRotateYMatrix(0.5f * std::clamp((pPlayer_->GetSwimmingDepth() - 10.0f) / 30.0f, -1.0f, 1.0f) *
						(pPlayer_->GetVelocity().x / 80.0f))))
				- transform_.translate;


			dif.y += pPlayer_->GetVelocity().y * 18.0f;
			transform_.translate.x += 6.0f * std::clamp(((pPlayer_->GetSwimmingDepth() - 10.0f) / -30.0f), 0.0f, 1.0f)
				* (pPlayer_->GetVelocity().x / 80.0f) * GameTimer::TimeRate();
		} else{


			dif = (target_->GetTranslation() +
				((offsetVec_ * offsetLength_) *
					MakeRotateYMatrix(0.5f * std::clamp((pPlayer_->GetSwimmingDepth() - 10.0f) / 30.0f, -1.0f, 1.0f))))
				- transform_.translate;

			transform_.translate.y -= 10.0f * ((pPlayer_->GetSwimmingDepth() - 10.0f) / 30.0f) * GameTimer::TimeRate();
		}

		// 少し遅らせて追従
		transform_.translate += dif * 0.1f * GameTimer::TimeRate();
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
