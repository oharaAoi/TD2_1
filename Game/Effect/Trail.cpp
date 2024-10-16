#include "Trail.h"

Trail::Trail() {
}

Trail::~Trail() {
}

void Trail::Init() {
	newLifeTime_ = 2.0f;
}

void Trail::Update() {
	for(std::list<TrailData>::iterator it = trails_.begin(); it != trails_.end();){
		// -------------------------------------------------
		// ↓ trailの位置をplayerに合わせる
		// -------------------------------------------------
		Vector3 trailPos = (*it).entity->GetTransform()->GetTranslation();
		trailPos = trailPos.Lerp(trailPos, playerPosition_, 0.05f);
		(*it).entity->GetTransform()->SetTranslaion(trailPos);

		(*it).entity->Update();
		(*it).entity->SetColor(Vector4(1.0f,1.0f,1.0f, std::lerp(0.0f, 1.0f, (*it).lifeTime_ / newLifeTime_)));
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

void Trail::AddTrail(const Vector3& pos) {
	if (pos.x == playerPosition_.x && pos.y == playerPosition_.y && pos.z == playerPosition_.z) {
		return;
	}

	if (createCoolTime_ <= 0.0f) {
 		createCoolTime_ = 0.2f;
		trails_.emplace_back(TrailData{ pos, newLifeTime_ });
	}
}
