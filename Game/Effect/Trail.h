#pragma once
#include "Engine/GameObject/BaseGameObject.h"

/// <summary>
/// Playerを後追いするEffect
/// </summary>
class Trail {
public:

	struct TrailData {
		std::unique_ptr<BaseGameObject> entity;
		float lifeTime_;

		TrailData(const Vector3& pos, float lifeTime) {
			entity = std::make_unique<BaseGameObject>();
			entity->Init();
			entity->SetObject("Item.obj");
			entity->GetTransform()->SetTranslaion(pos);
			lifeTime_ = lifeTime;
		}
	};

public:

	Trail();
	~Trail();

	void Init();
	void Update();
	void Draw() const;

	void AddTrail(const Vector3& pos);

	void SetPlayerPosition(const Vector3& playerPos) { playerPosition_ = playerPos; }

private:

	// 
	Vector3 playerPosition_;

	std::list<TrailData> trails_;
	float newLifeTime_;

	float createCoolTime_ = 3.0f;

};

