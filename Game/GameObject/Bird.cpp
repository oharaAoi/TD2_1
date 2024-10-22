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
	SetObject("Bird.gltf");
	SetAnimater("./Game/Resources/Model/Bird/", "Bird.gltf", true);
	transform_->SetScale({ 3.0f, 3.0f, 3.0f });
	radius_ = RandomFloat(6.0f, 10.0f);
	obb_.size = { radius_, radius_, radius_ };
	obb_.center = GetWorldTranslation();

	SetIsLighting(false);
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

void Bird::ApplyLoadData(const Vector3& scale, const Quaternion& rotate,
						 const Vector3& pos, const SubAttributeType& subType){
	BasePlacementObject::ApplyLoadData(scale, rotate, pos, subType);
	transform_->SetScale({ 3.0f, 3.0f, 3.0f });
    	firstPos_ = pos;
}

void Bird::IndividualFromCommon(const SubAttributeType& subType) {
	switch (subType) {
	case SubAttributeType::NONE:
		isMove_ = true;
		break;
	case SubAttributeType::NOTMOVE:
		isMove_ = false;
		break;
	}
}
