#include "DebugCamera.h"

DebugCamera::DebugCamera() {
	Init();
}

DebugCamera::~DebugCamera() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化
//////////////////////////////////////////////////////////////////////////////////////////////////

void DebugCamera::Finalize() {
}

void DebugCamera::Init() {
	transform_ = {
		{1.0f, 1.0f, 1.0f},
		{0 , 0, 0.0f},
		{0.0f, 5.0f, -30.0f}
	};

	// 行列の生成
	scaleMat_ = MakeScaleMatrix(transform_.scale);
	rotateMat_ = MakeRotateXYZMatrix(transform_.rotate);
	translateMat_ = MakeTranslateMatrix(transform_.translate);

	// worldの生成
	cameraMatrix_ = Multiply(Multiply(scaleMat_, rotateMat_), translateMat_);
	viewMatrix_ = Inverse(cameraMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth_) / float(kWindowHeight_), 0.1f, 100.0f);

	debugCameraMode_ = true;

	isMoveSpeed_ = 5.0f;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void DebugCamera::Update() {
	RotateMove();
	TransitionMove();

	scaleMat_ = MakeScaleMatrix(transform_.scale);
	rotateMat_ = quaternion_.MakeMatrix();
	translateMat_ = MakeTranslateMatrix(transform_.translate);

	cameraMatrix_ =scaleMat_ * rotateMat_ * translateMat_;
	viewMatrix_ = Inverse(cameraMatrix_);

	projectionMatrix_ = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth_) / float(kWindowHeight_), 0.1f, 100.0f);

	quaternion_.Normalize();
}

#ifdef _DEBUG
void DebugCamera::Debug_Gui() {
	ImGui::Begin("Debug_Camera");
	
	Vector3 right = quaternion_.MakeRight();
	Vector3 up = quaternion_.MakeUp();
	Vector3 forward = quaternion_.MakeForward();

	ImGui::DragFloat3("translate", &transform_.translate.x, 0.1f);
	ImGui::DragFloat4("rotate", &quaternion_.x, 0.01f);
	ImGui::DragFloat("isMoveSpeed", &isMoveSpeed_, 0.1f, 0.0f, 10.0f);
	ImGui::DragFloat("sensitivity", &sensitivity_, 0.1f, 0.0f, 10.0f);
	ImGui::DragFloat("kDedzorn", &dedzorn_, 1.0f, 0.0f, 20.0f);
	ImGui::Separator();
	ImGui::DragFloat3("right", &right.x, 0.1f);
	ImGui::DragFloat3("up", &up.x, 0.1f);
	ImGui::DragFloat("yaw", &yaw_, 0.1f);
	ImGui::DragFloat("pitch", &pitch_, 0.1f);

	if(ImGui::Button("Reset")) {
		transform_ = { {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 5.0f, -30.0f} };
		quaternion_ = Quaternion();
		yaw_ = 0.0f;
		pitch_ = 0.0f;
	}

	ImGui::End();
}
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　xy軸方向移動
//////////////////////////////////////////////////////////////////////////////////////////////////

void DebugCamera::TransitionMove() {
	
	moveDirection_ = Vector3();

	if (Input::IsPressKey(DIK_A)) {
		moveDirection_ -= quaternion_.MakeRight() * isMoveSpeed_;
	}

	if (Input::IsPressKey(DIK_D)) {
		moveDirection_ += quaternion_.MakeRight() * isMoveSpeed_;
	}

	if (Input::IsPressKey(DIK_W)) {
		moveDirection_ += quaternion_.MakeForward() * isMoveSpeed_;
	}

	if (Input::IsPressKey(DIK_S)) {
		moveDirection_ -= quaternion_.MakeForward() * isMoveSpeed_;
	}

	if (Input::IsPressKey(DIK_Q)) {
		moveDirection_ += quaternion_.MakeUp() * isMoveSpeed_;
	}

	if (Input::IsPressKey(DIK_E)) {
		moveDirection_ -= quaternion_.MakeUp() * isMoveSpeed_;
	}

	transform_.translate += moveDirection_ * GameTimer::DeltaTime();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　回転移動
//////////////////////////////////////////////////////////////////////////////////////////////////

void DebugCamera::RotateMove() {
	if (Input::IsPressMouse(1)) {
		Vector2 dire = Input::GetMousePosition() - preMousePos_;
		if (std::abs(dire.x) > dedzorn_) {
			yaw_ = dire.x * sensitivity_ * GameTimer::DeltaTime();
			Quaternion qYaw = Quaternion::AngleAxis(yaw_, quaternion_.MakeUp());
			quaternion_ = qYaw * quaternion_;
		}
		
		if (std::abs(dire.y) > dedzorn_) {
			pitch_ = dire.y * sensitivity_ * GameTimer::DeltaTime();
			Quaternion qPitch = Quaternion::AngleAxis(pitch_, quaternion_.MakeRight());
			quaternion_ = qPitch * quaternion_;
		}
	} 

	preMousePos_ = Input::GetMousePosition();
}