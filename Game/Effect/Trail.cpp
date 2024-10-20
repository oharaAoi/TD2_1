#include "Trail.h"

std::string Trail::useTrailName_;

Trail::Trail() {
}

Trail::~Trail() {
}

void Trail::Init() {
	newLifeTime_ = 1.5f;
}

void Trail::Update() {
	for (std::list<TrailData>::iterator it = trails_.begin(); it != trails_.end();) {
		if (!(*it).isFlying_) {
			// -------------------------------------------------
			// ↓ trailの位置をplayerに合わせる
			// -------------------------------------------------
			Vector3 trailPos = (*it).entity->GetTransform()->GetTranslation();
			Vector3 scale = (*it).entity->GetTransform()->GetScale();
			Quaternion quaternion = (*it).entity->GetTransform()->GetQuaternion();
			Quaternion moveQuaternion = Quaternion::AngleAxis(RandomFloat(5.0f, 10.0f) * toRadian, { 1.0f,0.0f,0.0f }).Normalize();
			(*it).entity->GetTransform()->SetQuaternion(moveQuaternion * quaternion);

			scale = scale.Lerp((*it).startScale_, { 0.0f, 0.0f, 0.0f }, 0.05f);
			(*it).entity->GetTransform()->SetTranslaion(trailPos);
		} else {

		}

		(*it).entity->Update();
		(*it).entity->SetColor(Vector4(1.0f, 1.0f, 1.0f, std::lerp(0.0f, 1.0f, (*it).lifeTime_ / newLifeTime_)));
		(*it).lifeTime_ -= GameTimer::DeltaTime();

		if ((*it).lifeTime_ < 0) {
			it = trails_.erase(it);
		} else {
			++it;
		}
	}

	if (createCoolTime_ != 0.0f) {
		createCoolTime_ -= GameTimer::DeltaTime();
		if (createCoolTime_ <= 0.0f) {
			createCoolTime_ = 0.0f;
		}
	}
}

void Trail::Draw() const {
	for (std::list<TrailData>::const_iterator it = trails_.begin(); it != trails_.end();) {
		(*it).entity->Draw();
		++it;
	}
}

void Trail::AddTrail(const Vector3& pos, const Quaternion& rotate, bool isFlying) {
	if (pos.x == playerPosition_.x && pos.y == playerPosition_.y && pos.z == playerPosition_.z) {
		return;
	}

	if (createCoolTime_ <= 0.0f) {
		if (isFlying) {
			useTrailName_ = "staer.obj";
		} else {
			useTrailName_ = "waterTrail.obj";
		}

 		createCoolTime_ = 0.1f;
		float randomFloat = RandomFloat(0.5f, 1.2f);
		Vector3 random = { RandomFloat(-5.0f, 5.0f), RandomFloat(-3.0f, 3.0f) , RandomFloat(-2.0f, 3.0f) };

		if (isFlying) {
			random = { 0.0f, 0.0f,0.0f };
			randomFloat = 1.0f;
		}

		trails_.emplace_back(TrailData{ pos + random ,
							 {randomFloat, randomFloat , randomFloat},
							 rotate,
							 newLifeTime_, isFlying });
	}
}
