#pragma once
#include <list>
#include "Game/Effect/FishEnergy.h"

class FishEnergyManager {
public:

	FishEnergyManager() = default;
	~FishEnergyManager();
	FishEnergyManager(const FishEnergyManager&) = delete;
	const FishEnergyManager& operator=(const FishEnergyManager&) = delete;

	static FishEnergyManager* GetInstance();

	void Finalize();

	void Init();

	void Update();

	void Draw() const;

public:

	void AddEnergy(const Matrix4x4& worldMatrix , uint32_t createNum);

	void SetVpvpMatrix(const Matrix4x4& matrix) { vpvpMatrix_ = matrix; }

	void SetTargetPos(const Vector2& pos) { targetPos_ = pos; }

private:

	std::list<std::unique_ptr<FishEnergy>> fishEnergyList_;
		
	Matrix4x4 vpvpMatrix_;

	Vector2 targetPos_;
};

