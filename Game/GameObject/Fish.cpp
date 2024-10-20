#include "Fish.h"

//////////////////////////////////////////////////////
//  コンストラクタ・デストラクタ
//////////////////////////////////////////////////////

Fish::Fish(){}
Fish::~Fish(){}

//////////////////////////////////////////////////////
//  初期化処理
//////////////////////////////////////////////////////

void Fish::Init(){

	typeID_ = (int)ObjectType::FISH;
	
	BaseGameObject::Init();
	radius_ = 1.0f;
	SetObject("Fish.gltf");
	SetAnimater("./Game/Resources/Model/Fish/", "Fish.gltf", true);

	obb_.size = { 1.0f, 1.0f, 2.0f };
	obb_.center = GetWorldTranslation();

	IndividualFromCommon();
}

//////////////////////////////////////////////////////
//  更新処理
//////////////////////////////////////////////////////

void Fish::Update(){
	IndividualFromCommon(subType_);
	transform_->SetQuaternion(Quaternion::AngleAxis((-90.0f * toRadian), Vector3{ 0.0, 1.0f, 0.0f }));
	obb_.center = GetWorldTranslation();
	obb_.MakeOBBAxis(transform_->GetQuaternion());
	BaseGameObject::Update();
}

//////////////////////////////////////////////////////
//  描画処理
//////////////////////////////////////////////////////

void Fish::Draw() const{
	BaseGameObject::Draw();
}

//////////////////////////////////////////////////////
//  衝突時処理
//////////////////////////////////////////////////////

void Fish::OnCollision(Collider* other){

	//プレイヤーに当たった場合
	if(other->GetObjectType() == int(ObjectType::PLAYER)){
		isActive_ = false;
	}
}

void Fish::IndividualFromCommon(const SubAttributeType& subType) {
	Vector3 scale{};

	switch (subType) {
	case SubAttributeType::NONE:

		break;
	case SubAttributeType::SMALL:
		fishSize_ = FISH_SIZE::SMALL;
		radius_ = 2.6f;
		energy_ = 0.075f;
		scale = { 2.0f,2.0f,2.0f };
		break;
	case SubAttributeType::MIDIUM:
		fishSize_ = FISH_SIZE::MIDIUM;
		radius_ = 4.0f;
		energy_ = 0.1f;
		scale = { 3.0f,3.0f,3.0f };
		break;
	case SubAttributeType::LARGE:
		fishSize_ = FISH_SIZE::LARGE;
		radius_ = 6.0f;
		energy_ = 0.2f;
		scale = { 6.0f,6.0f,6.0f };

		break;
	}

	Vector3 newSize = initSize * radius_*2;
	//float size = radius_ * 2.0f;
	obb_.size = newSize;
	transform_->SetScale(scale);
}

void Fish::SetFishSize(const FISH_SIZE& fishSize){
	fishSize_ = fishSize;

	Vector3 scale{};

	switch((int)fishSize_)
	{
	case (int)FISH_SIZE::SMALL:

		radius_ = 1.3f;
		energy_ = 0.075f;
		scale = { 2.0f,2.0f,2.0f };
		subType_ = SubAttributeType::SMALL;
		break;

	case (int)FISH_SIZE::MIDIUM:

		radius_ = 2.0f;
		energy_ = 0.1f;
		scale = { 3.0f,3.0f,3.0f };
		subType_ = SubAttributeType::MIDIUM;
		break;

	case (int)FISH_SIZE::LARGE:

		radius_ = 3.0f;
		energy_ = 0.2f;
		scale = { 6.0f,6.0f,6.0f };
		subType_ = SubAttributeType::LARGE;
		break;

	default:
		break;
	}

	float size = radius_ * 2.0f;
	obb_.size = { size, size, size };
	transform_->SetScale(scale);
}
