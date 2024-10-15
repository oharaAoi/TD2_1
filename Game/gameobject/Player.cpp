#include "Player.h"
#include "Game/Scene/GameScene.h"

Player::Player(){
	Init();
}

Player::~Player(){}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Init(){

	typeID_ = (int)ObjectType::PLAYER;

	BaseGameObject::Init();
	SetObject("skin.obj");
	aboveWaterSurfacePos = Engine::CreateWorldTransform();

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
	moveSpeed_ = 25.0f;//0.7f / (1.0f / 60.0f);
	radius_ = 1.0f;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Update(){
	if(isMove_) {
		Move();
	} else {
		velocity_ = { 0.0f,0.0f,0.0f };
	}

	BaseGameObject::Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Draw() const{
	BaseGameObject::Draw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　移動関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Move(){
	Vector3 translate = transform_->GetTranslation();
	if(Input::IsPressKey(DIK_SPACE)) {

		pressTime_ += 0.005f;

	} else {

		pressTime_ -= 0.005f;
	}

	// 角度を加算
	pressTime_ = std::clamp(pressTime_, -1.0f, 1.0f);
	float ease = EaseOutBack(std::fabsf(pressTime_));
	currentAngle_ = kMaxAngle_ * ease * (pressTime_ > 0.0f ? 1.0f : -1.0f);
	LookAtDirection(currentAngle_);

	// 移動量を加算
	velocity_ = Vector3(1.0f,0.0f,0.0f) * MakeRotateZMatrix(currentAngle_);
	velocity_ *= moveSpeed_ * std::fabsf(GameTimer::DeltaTime());
	transform_->SetTranslaion(translate + velocity_);

	// プレイヤー上部の水面の座標を取得
	aboveWaterSurfacePos->SetTranslaion({ transform_->GetTranslation().x, 10.0f,0.0f });
	aboveWaterSurfacePos->Update();

	// 深さを更新
	swimmigDepth_ = 10.0f - transform_->GetTranslation().y;

	// 飛行フラグ更新
	if(transform_->GetTranslation().y > 0.0f){
		isFlying_ = true;
	} else{
		isFlying_ = false;
	}

	MoveLimit();
}


void Player::MoveLimit(){

	// 地面に接触したら
	if(transform_->GetTranslation().y - radius_ < GameScene::GetGroundDepth()){
		Vector3 translate = transform_->GetTranslation();
		transform_->SetTranslaion({ translate.x,GameScene::GetGroundDepth() + radius_,translate.z });
		pressTime_ = 0.0f;
		moveSpeed_ -= moveSpeed_ * 0.5f * GameTimer::DeltaTime();
	}

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

void Player::AdaptAdjustmentItem(){
	const char* groupName = "Player";
	transform_->SetTranslaion(adjustmentItem_->GetValue<Vector3>(groupName, "position"));
	moveSpeed_ = adjustmentItem_->GetValue<float>(groupName, "moveSpeed");
	velocity_ = adjustmentItem_->GetValue<Vector3>(groupName, "velocity");
	lookAtT_ = adjustmentItem_->GetValue<float>(groupName, "lookAtT");
	radius_ = adjustmentItem_->GetValue<float>(groupName, "radius");
}

const Vector3 Player::GetWorldTranslation(const Vector3& offset) const{
	return Transform(offset, transform_->GetWorldMatrix());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void Player::Debug_Gui(){
	ImGui::Begin("Player");
	transform_->Debug_Gui();
	ImGui::Separator();
	ImGui::Text("Parameter");
	ImGui::DragFloat3("velocity", &velocity_.x, 0.1f);
	ImGui::DragFloat("moveSpeed", &moveSpeed_, 0.1f);
	ImGui::DragFloat("lookAtT", &lookAtT_, 0.1f);
	ImGui::DragFloat("radius", &radius_, 0.1f);

	if(ImGui::Button("ReAdapt")) {
		AdaptAdjustmentItem();
	}


	ImGui::Checkbox("isMove", &isMove_);

	if(hitWaterSurface_) {
		ImGui::Text("Hit");
	} else {
		ImGui::Text("not Hit");
	}

	ImGui::End();
}
#endif // _DEBUG

//////////////////////////////////////////////////////
//  衝突時処理
//////////////////////////////////////////////////////

void Player::OnCollision(Collider* other){

	int ownerType = int(other->GetObjectType() & (int)ObjectType::OBSTACLE);

	//障害物に当たった場合
	if(ownerType == (int)ObjectType::OBSTACLE){
		moveSpeed_ -= 10.0f;
		moveSpeed_ = std::clamp(moveSpeed_, 10.0f, 100.0f);
	}

	if(other->GetObjectType() == (int)ObjectType::ITEM){
		moveSpeed_ += 10.0f;
		moveSpeed_ = std::clamp(moveSpeed_, 10.0f, 100.0f);
	}
}