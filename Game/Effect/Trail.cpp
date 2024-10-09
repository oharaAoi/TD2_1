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
		(*it).entity->Update();
		(*it).entity->SetColor(Vector4(1.0f,1.0f,1.0f, std::lerp(0.0f, 1.0f, (*it).lifeTime_ / newLifeTime_)));
		(*it).lifeTime_ -= 1.0f * GameTimer::DeltaTime();

		if ((*it).lifeTime_ < 0) {
			it = trails_.erase(it);
		} else {
			++it;
		}
	}

	if (createCoolTime_ != 0.0f) {
		createCoolTime_ -= 1.0f * GameTimer::DeltaTime();
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
	if (createCoolTime_ <= 0.0f) {
		createCoolTime_ = 0.1f;
		trails_.emplace_back(TrailData{ pos, newLifeTime_ });
	}
}
