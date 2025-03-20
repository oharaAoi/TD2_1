#include "FishEnergyManager.h"

FishEnergyManager::~FishEnergyManager() {
	fishEnergyList_.clear();
}

FishEnergyManager* FishEnergyManager::GetInstance() {
	static FishEnergyManager instance;
	return &instance;
}

void FishEnergyManager::Finalize() {
	fishEnergyList_.clear();
}

void FishEnergyManager::Init() {
	fishEnergyList_.clear();
}

void FishEnergyManager::Update() {
	fishEnergyList_.remove_if([](const std::unique_ptr<FishEnergy>& fish) {
		return !fish->GetIsAlive();
	});

	for (auto& energy : fishEnergyList_) {
		energy->SetTargetPos(targetPos_);
		energy->Update();
	}
}

void FishEnergyManager::Draw() const {
	for (const auto& energy : fishEnergyList_) {
		energy->Draw();
	}
}

void FishEnergyManager::AddEnergy(const Matrix4x4& worldMatrix, uint32_t createNum) {
	Vector3 screenPos = Transform({ 0.0f, 0.0f, 0.0f },worldMatrix* vpvpMatrix_);

	for (uint32_t oi = 0; oi < createNum; ++oi) {
		auto& energy = fishEnergyList_.emplace_back(std::make_unique<FishEnergy>());
		energy->Init(Vector2(screenPos.x, screenPos.y));
	}
}
