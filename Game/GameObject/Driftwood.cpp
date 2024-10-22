#include "Driftwood.h"

//////////////////////////////////////////////////////
//  コンストラクタ・デストラクタ
//////////////////////////////////////////////////////

Driftwood::Driftwood() { Init(); }
Driftwood::~Driftwood() {}

//////////////////////////////////////////////////////
//  初期化処理
//////////////////////////////////////////////////////

void Driftwood::Init() {

	typeID_ = (int)ObjectType::DRIFTWOOD;

	BaseGameObject::Init();
	SetObject("Driftwood2.obj");

	radius_ = 8.0f;
	obb_.size = { radius_ * 3.5f , radius_ , 2.5f };
	obb_.center = GetWorldTranslation();

	radius_ = obb_.size.x;
	SetIsLighting(true);
}

//////////////////////////////////////////////////////
//  更新処理
//////////////////////////////////////////////////////

void Driftwood::Update() {
	obb_.center = GetWorldTranslation();
	obb_.MakeOBBAxis(transform_->GetQuaternion());
	BaseGameObject::Update();
}

//////////////////////////////////////////////////////
//  描画処理
//////////////////////////////////////////////////////

void Driftwood::Draw() const {
	BaseGameObject::Draw();
}

//////////////////////////////////////////////////////
//  衝突時処理
//////////////////////////////////////////////////////

void Driftwood::OnCollision(Collider* other) {

	//プレイヤーに当たった場合
	if (other->GetObjectType() == int(ObjectType::PLAYER)) {
		isActive_ = false;

		AnimetionEffectManager::AddListEffect("./Game/Resources/Model/DriftWoodDestroy/", "DriftWoodDestroy.gltf",
											  transform_.get(), true, transform_->GetTranslation(), transform_->GetQuaternion(), transform_->GetTranslation());
	}
}

void Driftwood::IndividualFromCommon(const SubAttributeType& subType) {
	switch (subType) {
	case SubAttributeType::NONE:
		break;
	}
}
