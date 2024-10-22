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
		radius_ = 10.0f * 1.3f;
		obb_.size = { (float)RockSize::SMALL,(float)RockSize::SMALL * 1.3f,(float)RockSize::SMALL };
		obb_.center = GetWorldTranslation();
		scale = { 1.0f,1.0f,1.0f };
		break;

	case SubAttributeType::MIDIUM:
		radius_ = 15.0f * 1.3f;
		obb_.size = { (float)RockSize::MIDIUM, (float)RockSize::MIDIUM * 1.3f, (float)RockSize::MIDIUM };
		obb_.center = GetWorldTranslation();
		scale = { 2.5f,2.5f,2.5f };
		break;

	case SubAttributeType::LARGE:
		radius_ = 18.0f * 1.3f;
		obb_.size = { (float)RockSize::LARGE, (float)RockSize::LARGE * 1.3f, (float)RockSize::LARGE };
		obb_.center = GetWorldTranslation();
		scale = { 4,4,4 };
		break;
	}
	transform_->SetScale(scale);
}
