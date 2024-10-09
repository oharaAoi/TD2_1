#include "Player.h"

Player::Player() {
	Init();
}

Player::~Player() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Init() {
	BaseGameObject::Init();
	SetObject("skin.obj");

	adjustmentItem_ = AdjustmentItem::GetInstance();
	const char* groupName = "Player";
	adjustmentItem_->AddItem(groupName, "position", transform_->GetTranslation());
	adjustmentItem_->AddItem(groupName, "velocity", velocity_);
	adjustmentItem_->AddItem(groupName, "moveSpeed", moveSpeed_);
	adjustmentItem_->AddItem(groupName, "lookAtT", lookAtT_);
	adjustmentItem_->AddItem(groupName, "radius", radius_);

	AdaptAdjustmentItem();

	restPoseRotation_ = Quaternion::AngleAxis(90.0f * toRadian, Vector3(0.0f, 1.0f, 0.0f));
	transform_->SetQuaternion(restPoseRotation_);

	isMove_ = false;

	radius_ = 1.0f;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Update() {
	if (isMove_) {
		velocity_.x = 1.0f;
		Move();
	} else {
		velocity_.x = 0.0f;
	}

	BaseGameObject::Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Draw() const {
	BaseGameObject::Draw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　移動関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Move() {
	Vector3 translate = transform_->GetTranslation();
	if (Input::IsPressKey(DIK_SPACE)) {
		// 移動処理
		velocity_.y = 1.0f;
		velocity_.Normalize();
		translate += velocity_ * moveSpeed_ * GameTimer::DeltaTime();

		// 回転処理
		float targetAngle = std::atan2f(velocity_.x, velocity_.y);
		LookAtDirection(targetAngle);

	} else {
		// 移動処理
		velocity_.y = -1.0f;
		velocity_.Normalize();
		translate += velocity_ * moveSpeed_ * GameTimer::DeltaTime();

		// 回転処理
		float targetAngle = std::atan2f(velocity_.y, velocity_.x);
		LookAtDirection(targetAngle);
	}
	transform_->SetTranslaion(translate);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　進行方向を向かせる
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::LookAtDirection(const float& angle) {
	Quaternion moveRotation = Quaternion::EulerToQuaternion(Vector3(0.0f, 0.0f, angle).Normalize()) * restPoseRotation_;
	Quaternion slerp = Quaternion::Slerp(transform_->GetQuaternion().Normalize(), moveRotation.Normalize(), lookAtT_).Normalize();
	transform_->SetQuaternion(slerp);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　調整項目を適応させる
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::AdaptAdjustmentItem() {
	const char* groupName = "Player";
	transform_->SetTranslaion(adjustmentItem_->GetValue<Vector3>(groupName, "position"));
	moveSpeed_ = adjustmentItem_->GetValue<float>(groupName, "moveSpeed");
	velocity_ = adjustmentItem_->GetValue<Vector3>(groupName, "velocity");
	lookAtT_ = adjustmentItem_->GetValue<float>(groupName, "lookAtT");
	radius_ = adjustmentItem_->GetValue<float>(groupName, "radius");
}

const Vector3 Player::GetWorldTranslation(const Vector3& offset) const {
	return Transform(offset, transform_->GetWorldMatrix());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void Player::Debug_Gui() {
	ImGui::Begin("Player");
	transform_->Debug_Gui();
	ImGui::Separator();
	ImGui::Text("Parameter");
	ImGui::DragFloat3("velocity", &velocity_.x, 0.1f);
	ImGui::DragFloat("moveSpeed", &moveSpeed_, 0.1f);
	ImGui::DragFloat("lookAtT", &lookAtT_, 0.1f);
	ImGui::DragFloat("radius", &radius_, 0.1f);

	if (ImGui::Button("ReAdapt")) {
		AdaptAdjustmentItem();
	}


	ImGui::Checkbox("isMove", &isMove_);

	if (hitWaterSurface_) {
		ImGui::Text("Hit");
	} else {
		ImGui::Text("not Hit");
	}

	ImGui::End();
}
#endif // _DEBUG