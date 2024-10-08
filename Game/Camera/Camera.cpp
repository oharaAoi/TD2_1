#include "Camera.h"

Camera::Camera() {
	Init();
}

Camera::~Camera() {
}

void Camera::Finalize() {
}

void Camera::Init() {
	BaseCamera::Init();

	transform_ = {
		{1.0f, 1.0f, 1.0f},
		{0.0f, 0.91f, 0.0f},
		{-20.4f, 0.3f, -25.7f}
	};

	offset_ = transform_.translate;
}

void Camera::Update() {

	// -------------------------------------------------
	// ↓ Targetがいたら
	// -------------------------------------------------
	if (target_ != nullptr) {
		// 目的座標への差分
		Vector3 dif = (target_->GetTranslation() + offset_) - transform_.translate;

		// 少し遅らせて追従
		transform_.translate += dif * 0.05f;
	}

	BaseCamera::Update();
}

#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
void Camera::Debug_Gui() {
	ImGui::Begin("Camera");
	if (ImGui::Button("Reset")) {
		transform_ = { {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -9.0f} };
		transform_.rotate = { 0.0f, 0.0f, 0.0f };
		rotateMat_ = MakeRotateXYZMatrix(transform_.rotate);
	}

	ImGui::DragFloat3("translate", &transform_.translate.x, 0.1f);
	ImGui::DragFloat3("rotate", &transform_.rotate.x, 0.1f);

	ImGui::End();
}
#endif
