#include "Coin.h"

//////////////////////////////////////////////////////
//  コンストラクタ・デストラクタ
//////////////////////////////////////////////////////

Coin::Coin() { Init(); }
Coin::~Coin() {}

//////////////////////////////////////////////////////
//  初期化処理
//////////////////////////////////////////////////////

void Coin::Init() {

	typeID_ = (int)ObjectType::COIN;

	BaseGameObject::Init();
	SetObject("Coin.gltf");
	SetAnimater("./Game/Resources/Model/Coin/", "Coin.gltf", true);
	SetColor({ 1.0f, 1.0f, 0.0f, 1.0f });

	obb_.size = { 1.0f, 1.0f, 1.0f };
	obb_.center = GetWorldTranslation();
}

//////////////////////////////////////////////////////
//  更新処理
//////////////////////////////////////////////////////

void Coin::Update() {
	obb_.center = GetWorldTranslation();
	obb_.MakeOBBAxis(transform_->GetQuaternion());
	BaseGameObject::Update();
}

//////////////////////////////////////////////////////
//  描画処理
//////////////////////////////////////////////////////

void Coin::Draw() const {
	BaseGameObject::Draw();
}

//////////////////////////////////////////////////////
//  衝突時処理
//////////////////////////////////////////////////////

void Coin::OnCollision(Collider* other) {
	//プレイヤーに当たった場合
	if (other->GetObjectType() == int(ObjectType::PLAYER)) {
		isActive_ = false;
	}
}

void Coin::IndividualFromCommon(const SubAttributeType& subType) {
	switch (subType) {
	case SubAttributeType::NONE:
		break;
	}
}
