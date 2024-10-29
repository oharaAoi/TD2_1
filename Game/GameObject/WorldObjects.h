#pragma once
#include "Game/Information/StageInformation.h"
#include "Game/GameObject/WorldWall.h"
#include "Game/GameObject/Mountain.h"
#include "Game/GameObject/Ground.h"
#include "Game/GameObject/WaterSpace.h"
#include "Game/GameObject/Waterweed.h"
#include "Game/GameObject/Player.h"

class WorldObjects {
public:

	WorldObjects();
	~WorldObjects();

	void Init();
	void Update(float playerPos);
	void Draw() const;
	void DrawWater() const;

	void LoopStage();

	void CollisionPlayerToWaterSpace(Player* player);

private:

	float playerPos_;

	// ---------- worldobject ---------- //
	static const uint32_t kStageMax_ = 2;
	uint32_t stageLoopCount_ = 0;
	bool nowStageIndex_ = 0;
	std::unique_ptr<WorldWall> worldWalls_[kStageMax_];
	std::unique_ptr<Waterweed> waterWeeds_[kStageMax_];
	std::unique_ptr<Mountain> mountains_[kStageMax_];

	std::unique_ptr<Ground> grounds_[kStageMax_];
	std::unique_ptr<WaterSpace> waterSpaces_[kStageMax_];

	uint32_t mountainLoopCount_ = 0;
	bool nowMountainIndex_ = 0;
	std::unique_ptr<BaseGameObject> trees_[kStageMax_];
	std::unique_ptr<BaseGameObject> grass_[kStageMax_];
	std::unique_ptr<BaseGameObject> cloud_[kStageMax_];

	std::unique_ptr<BaseGameObject> moai_[kStageMax_];
	std::unique_ptr<BaseGameObject> nico_[kStageMax_];
	std::unique_ptr<BaseGameObject> ufo_[kStageMax_];
	std::unique_ptr<BaseGameObject> ufosmoke_[kStageMax_];

	float cloudZ;
	float time;
};

