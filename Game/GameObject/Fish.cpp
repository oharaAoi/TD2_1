#include "Fish.h"
#include <Game/Manager/AnimetionEffectManager.h>

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

	SetIsLighting(false);

	IndividualFromCommon();
}

//////////////////////////////////////////////////////
//  更新処理
//////////////////////////////////////////////////////

void Fish::Update(){
	Change();

	IndividualFromCommon(subType_);
	transform_->SetQuaternion(Quaternion::AngleAxis((-90.0f * toRadian), Vector3{ 0.0, 1.0f, 0.0f }));
	obb_.center = GetWorldTranslation();
	obb_.MakeOBBAxis(transform_->GetQuaternion());
	BaseGameObject::Update();

	isPreAte_ = isAte_;
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
		AnimetionEffectManager::AddListEffect("./Game/Resources/Model/FishDestroy/", "FishDestroy.gltf",
			nullptr, false, transform_.get()->GetScale(), Quaternion(), GetWorldTranslation());
	}
}

void Fish::IndividualFromCommon(const SubAttributeType& subType) {
	Vector3 scale{};

	switch (subType) {
	case SubAttributeType::NONE:
		subType_ = SubAttributeType::SMALL;
		break;
	case SubAttributeType::SMALL:
		fishSize_ = FISH_SIZE::SMALL;
		radius_ = 2.3f;
		energy_ = 0.075f;
		scale = { 1.5f,1.5f,1.5f };
		eatCount = 0;
		break;
	case SubAttributeType::MIDIUM:
		fishSize_ = FISH_SIZE::MIDIUM;
		radius_ = 3.5f;
		energy_ = 0.1f;
		scale = { 3.5f,3.5f,3.5f };
		eatCount = 4;
		break;
	case SubAttributeType::LARGE:
		fishSize_ = FISH_SIZE::LARGE;
		radius_ = 5;
		energy_ = 0.2f;
		scale = { 5,5,5 };
		eatCount = 8;
		break;
	}
	radius_ *= 2;
	Vector3 newSize = initSize * radius_;
	//float size = radius_ * 2.0f;
	obb_.size = newSize;
	transform_->SetScale(scale);
}

void Fish::Change() {
	if ((isPreAte_ == false) && (isAte_ == true)) {
		// 食べられる
		SetObject("EatFish.gltf");
		SetAnimater("./Game/Resources/Model/EatFish/", "EatFish.gltf", true);
		SetIsLighting(false);
	} else if ((isPreAte_ == true) && (isAte_ == false)) {
		// 食べられない
		SetObject("Fish.gltf");
		SetAnimater("./Game/Resources/Model/Fish/", "Fish.gltf", true);
		SetIsLighting(false);
	}
}

void Fish::SetFishSize(const FISH_SIZE& fishSize){
	fishSize_ = fishSize;

	Vector3 scale{};

	switch((int)fishSize_)
	{
	case (int)FISH_SIZE::SMALL:

		radius_ = 10.0f;
		energy_ = 0.075f;
		scale = { 2.0f,2.0f,2.0f };
		subType_ = SubAttributeType::SMALL;
		break;

	case (int)FISH_SIZE::MIDIUM:

		radius_ = 10.0f;
		energy_ = 0.1f;
		scale = { 3.0f,3.0f,3.0f };
		subType_ = SubAttributeType::MIDIUM;
		break;

	case (int)FISH_SIZE::LARGE:

		radius_ = 10.0f;
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
