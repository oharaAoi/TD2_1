#include "Waterweed.h"

//////////////////////////////////////////////////////
//  コンストラクタ・デストラクタ
//////////////////////////////////////////////////////

Waterweed::Waterweed() { Init(); }
Waterweed::~Waterweed() {}

//////////////////////////////////////////////////////
//  初期化処理
//////////////////////////////////////////////////////

void Waterweed::Init() {

	typeID_ = (int)ObjectType::WATERWEED;

	BaseGameObject::Init();
	SetObject("Ground_WaterPlant.obj");
	SetColor({ 0.0f, 1.0f, 0.0f, 1.0f });
	SetIsLighting(false);
	radius_ = 2.5f;
}

//////////////////////////////////////////////////////
//  更新処理
//////////////////////////////////////////////////////

void Waterweed::Update() {
	BaseGameObject::Update();
}

//////////////////////////////////////////////////////
//  描画処理
//////////////////////////////////////////////////////

void Waterweed::Draw() const {
	BaseGameObject::Draw();
}

//////////////////////////////////////////////////////
//  衝突時処理
//////////////////////////////////////////////////////

void Waterweed::OnCollision(Collider* other) {
	//プレイヤーに当たった場合
	if (other->GetObjectType() == int(ObjectType::PLAYER)) {
		//isActive_ = false;
	}
}

void Waterweed::SetWorldWallPos(const Vector3& pos) {
	GetTransform()->SetTranslaion(pos);
}

void Waterweed::IndividualFromCommon(const SubAttributeType& subType) {
	switch (subType) {
	case SubAttributeType::NONE:
		break;
	}
}
