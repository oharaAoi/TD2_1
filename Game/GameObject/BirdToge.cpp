#include "BirdToge.h"
#include <Game/Manager/AnimetionEffectManager.h>
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/Lib/GameTimer.h"
#include "Easing.h"
#include "Engine/Math/MyRandom.h"

BirdToge::BirdToge() {
	Init();
}

BirdToge::~BirdToge() {
}

void BirdToge::Init() {
	typeID_ = (int)ObjectType::BIRDTOGE;

	BaseGameObject::Init();
	SetObject("BirdToge.gltf");
	SetAnimater("./Game/Resources/Model/BirdToge/", "BirdToge.gltf", true);
	transform_->SetScale({ 8, 8, 8 });
	radius_ = 13;
	obb_.size = { radius_ * 0.6f, radius_, radius_ * 0.6f };
	obb_.center = GetWorldTranslation();

	SetIsLighting(false);
}

void BirdToge::Update() {
	typeID_ = (int)ObjectType::BIRDTOGE;

	transform_->SetTranslaion(bird_->GetWorldTranslation());

	obb_.center = GetWorldTranslation();
	obb_.MakeOBBAxis(transform_->GetQuaternion());
	BaseGameObject::Update();
}

void BirdToge::Draw() const {
	BaseGameObject::Draw();
}

void BirdToge::OnCollision(Collider* other) {
	if (other->GetObjectType() == int(ObjectType::PLAYER)) {
		isHitPlayer_ = true;
		isActive_ = false;
		AnimetionEffectManager::AddListEffect("./Game/Resources/Model/FishDestroy/", "FishDestroy.gltf",
											  nullptr, false, transform_.get()->GetScale() * 0.5f, Quaternion(), GetWorldTranslation());
	}
}

void BirdToge::ApplyLoadData(const Vector3& scale, const Quaternion& rotate,
							 const Vector3& pos, const SubAttributeType& subType) {
	BasePlacementObject::ApplyLoadData(scale, rotate, pos, subType);
	transform_->SetScale({ 6, 6, 6 });
	firstPos_ = pos;
	typeID_ = (int)ObjectType::BIRDTOGE;
}


void BirdToge::IndividualFromCommon(const SubAttributeType& subType) {
	switch (subType) {
	case SubAttributeType::NONE:
		isMove_ = true;
		break;
	case SubAttributeType::NOTMOVE:
		isMove_ = false;
		break;
	}
}

void BirdToge::ScaleChange(bool playerCloseWind) {
	if (playerCloseWind) {
		transform_->SetScale({ 0, 0, 0 });
	} else {
		transform_->SetScale(bird_->GetTransform()->GetScale());
	}
}