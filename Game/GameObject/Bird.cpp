#include "Bird.h"


//////////////////////////////////////////////////////
//  コンストラクタ・デストラクタ
//////////////////////////////////////////////////////

Bird::Bird() { Init(); }
Bird::~Bird() {}

//////////////////////////////////////////////////////
//  初期化処理
//////////////////////////////////////////////////////

void Bird::Init() {

	typeID_ = (int)ObjectType::BIRD;

	BaseGameObject::Init();
	SetObject("Bird.obj");

	obb_.size = { 1.0f, 1.0f, 1.0f };
	obb_.center = GetWorldTranslation();
}

//////////////////////////////////////////////////////
//  更新処理
//////////////////////////////////////////////////////

void Bird::Update() {

	if(isMove_){
		float move_t = std::sinf(3.14f * time_);// -1.0 ~ 1.0 になる
		transform_->SetTranslaion(firstPos_ + (moveDirection_.Normalize() * (moveRadius_ * move_t)));
		time_ += GameTimer::DeltaTime();
	}

	obb_.center = GetWorldTranslation();
	obb_.MakeOBBAxis(transform_->GetQuaternion());
	BaseGameObject::Update();
}

//////////////////////////////////////////////////////
//  描画処理
//////////////////////////////////////////////////////

void Bird::Draw() const {
	BaseGameObject::Draw();
}

//////////////////////////////////////////////////////
//  衝突時処理
//////////////////////////////////////////////////////

void Bird::OnCollision(Collider* other) {

	//プレイヤーに当たった場合
	if (other->GetObjectType() == int(ObjectType::PLAYER)) {
		isHitPlayer_ = true;
		isActive_ = false;
	}
}

void Bird::ApplyLoadData(const Vector3& scale, const Quaternion& rotate, const Vector3& pos, const float& radius){
	BasePlacementObject::ApplyLoadData(scale, rotate, pos, radius);
	firstPos_ = pos;
}
