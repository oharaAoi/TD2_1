#pragma once
#include "Engine/GameObject/BaseGameObject.h"
#include "Engine/Math/MyRandom.h"

/// <summary>
/// Playerを後追いするEffect
/// </summary>
class Trail {
public:

	struct TrailData {
		std::unique_ptr<BaseGameObject> entity;
		Vector3 startScale_;
		float lifeTime_;
		bool isFlying_;

		TrailData(const Vector3& pos, const Vector3& scale, const Quaternion& rotate, float lifeTime, bool isFlying) {
			entity = std::make_unique<BaseGameObject>();
			entity->Init();
			entity->SetObject(useTrailName_);
			entity->GetTransform()->SetTranslaion(pos);
			entity->GetTransform()->SetQuaternion(rotate);
			entity->GetTransform()->SetScale(scale);
			startScale_ = scale;
			lifeTime_ = lifeTime;
			isFlying_ = isFlying;
		}
	};

public:

	Trail();
	~Trail();

	void Init();
	void Update();
	void Draw() const;

	void AddTrail(const Vector3& pos, const Quaternion& rotate, bool isFlying);

	void SetPlayerPosition(const Vector3& playerPos) { playerPosition_ = playerPos; }

	void SetTrailObjName(const std::string& name) { useTrailName_ = name; }

private:

	// 
	Vector3 playerPosition_;

	std::list<TrailData> trails_;
	float newLifeTime_;

	float createCoolTime_ = 3.0f;

	static std::string useTrailName_;

};

