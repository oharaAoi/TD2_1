#include "Rock.h"

//////////////////////////////////////////////////////
//  コンストラクタ・デストラクタ
//////////////////////////////////////////////////////

Rock::Rock() { Init(); }
Rock::~Rock() {}

//////////////////////////////////////////////////////
//  初期化処理
//////////////////////////////////////////////////////

void Rock::Init() {

	typeID_ = (int)ObjectType::ROCK;

	BaseGameObject::Init();
	SetObject("Rock.obj");

	radius_ = 4.0f;
	obb_.size = { radius_, radius_ * 2.0f, radius_ };
	obb_.center = GetWorldTranslation();
	SetIsLighting(true);
}

//////////////////////////////////////////////////////
//  更新処理
//////////////////////////////////////////////////////

void Rock::Update() {
	IndividualFromCommon(subType_);
	obb_.center = GetWorldTranslation();
	obb_.MakeOBBAxis(transform_->GetQuaternion());
	obb_.center.y += radius_;
	BaseGameObject::Update();
}

//////////////////////////////////////////////////////
//  描画処理
//////////////////////////////////////////////////////

void Rock::Draw() const {
	BaseGameObject::Draw();
}

//////////////////////////////////////////////////////
//  衝突時処理
//////////////////////////////////////////////////////

void Rock::OnCollision(Collider* other) {

	//プレイヤーに当たった場合
	if (other->GetObjectType() == int(ObjectType::PLAYER)) {
		isActive_ = false;
	}
}

void Rock::IndividualFromCommon(const SubAttributeType& subType) {
	Vector3 scale{};
	switch (subType) {
	case SubAttributeType::NONE:
		break;
	case SubAttributeType::SMALL:
		radius_ = 3.0f;
		obb_.size = { radius_ * 2.0f, radius_ * 1.5f, radius_ };
		obb_.center = GetWorldTranslation();
		obb_.center.y += radius_;
		scale = { 1.0f,1.0f,1.0f };
		break;

	case SubAttributeType::MIDIUM:
		radius_ = 6.0f;
		obb_.size = { radius_ * 2.0f, radius_ * 1.5f, radius_ };
		obb_.center = GetWorldTranslation();
		obb_.center.y += radius_;
		scale = { 2.5f,2.5f,2.5f };
		break;

	case SubAttributeType::LARGE:
		radius_ = 10.0f;
		obb_.size = { radius_ * 2.0f, radius_ * 1.5f, radius_ };
		obb_.center = GetWorldTranslation();
		obb_.center.y += radius_;
		scale = { 4,4,4 };
		break;
	}
	transform_->SetScale(scale);
}
