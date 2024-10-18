#include "Fish.h"

//////////////////////////////////////////////////////
//  コンストラクタ・デストラクタ
//////////////////////////////////////////////////////

Fish::Fish(){ Init(); }
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

	obb_.size = { 1.0f, 1.0f, 1.0f };
	obb_.center = GetWorldTranslation();
}

//////////////////////////////////////////////////////
//  更新処理
//////////////////////////////////////////////////////

void Fish::Update(){
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

void Fish::SetFishSize(const FISH_SIZE& fishSize){
	fishSize_ = fishSize;
	
	switch((int)fishSize_)
	{
	case (int)FISH_SIZE::SMALL:

		radius_ = 1.0f;
		energy_ = 0.05f;
		break;

	case (int)FISH_SIZE::MIDIUM:

		radius_ = 3.0f;
		energy_ = 0.1f;
		break;

	case (int)FISH_SIZE::LARGE:

		radius_ = 5.0f;
		energy_ = 0.2f;

		break;

	default:
		break;
	}

	float size = radius_ * 0.5f;
	obb_.size = { size, size, size };
	transform_->SetScale({ radius_,radius_,radius_ });
}
