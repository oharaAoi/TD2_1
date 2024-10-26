#include "WorldObjects.h"

WorldObjects::WorldObjects() {
}

WorldObjects::~WorldObjects() {
	for (uint32_t oi = 0; oi < kStageMax_; ++oi) {
		waterSpaces_[oi]->Finalize();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void WorldObjects::Init() {
	cloudZ = 130.0f;

	for (uint32_t oi = 0; oi < kStageMax_; ++oi) {
		Vector3 newPos = StageInformation::worldWallPos_;
		newPos.x += StageInformation::stageWidthEvery_ * (oi);

		Vector3 newMountainPos = { 0.0f, 0.0f, 0.0f };
		newMountainPos.x += (StageInformation::stageWidthEvery_ * 2.0f) * (oi);

		worldWalls_[oi] = std::make_unique<WorldWall>();
		worldWalls_[oi]->Init();
		// 水草
		waterWeeds_[oi] = std::make_unique<Waterweed>();
		waterWeeds_[oi]->Init();
		waterWeeds_[oi]->SetWorldWallPos(StageInformation::worldWallPos_);
		// 地面
		grounds_[oi] = std::make_unique<Ground>();
		grounds_[oi]->GetTransform()->SetTranslaion(Vector3(0.0f, StageInformation::groundDepth_, 0.0f));
		// 水面
		waterSpaces_[oi] = std::make_unique<WaterSpace>();
		waterSpaces_[oi]->Init("./Game/Resources/Model/Watersurface/", "Watersurface.obj");
		// 山
		mountains_[oi] = std::make_unique<Mountain>();
		mountains_[oi]->Init();
		mountains_[oi]->SetIsLighting(true);

		// 木
		trees_[oi] = std::make_unique<BaseGameObject>();
		trees_[oi]->Init();
		trees_[oi]->SetObject("MountenTree.obj");
		trees_[oi]->SetIsLighting(false);
		// 草
		grass_[oi] = std::make_unique<BaseGameObject>();
		grass_[oi]->Init();
		grass_[oi]->SetObject("MountainGrass.obj");
		grass_[oi]->SetIsLighting(false);
		//雲
		cloud_[oi] = std::make_unique<BaseGameObject>();
		cloud_[oi]->Init();
		cloud_[oi]->SetObject("Cloud.obj");
		cloud_[oi]->SetColor({ 1.0f,1.0f,1.0f,0.5f });
		cloud_[oi]->GetTransform()->SetTranslationZ(cloudZ);
		cloud_[oi]->SetIsLighting(false);
		// moai
		moai_[oi] = std::make_unique<BaseGameObject>();
		moai_[oi]->Init();
		moai_[oi]->SetObject("Moai.obj");
		moai_[oi]->SetIsLighting(false);
		// nico
		nico_[oi] = std::make_unique<BaseGameObject>();
		nico_[oi]->Init();
		nico_[oi]->SetObject("Nico.obj");
		nico_[oi]->SetIsLighting(false);
		// ufo
		ufo_[oi] = std::make_unique<BaseGameObject>();
		ufo_[oi]->Init();
		ufo_[oi]->SetObject("MountainUFO.obj");
		ufo_[oi]->SetIsLighting(false);
		// ufosmoke
		ufosmoke_[oi] = std::make_unique<BaseGameObject>();
		ufosmoke_[oi]->Init();
		ufosmoke_[oi]->SetObject("UFOSmoke.obj");
		ufosmoke_[oi]->SetIsLighting(false);

		worldWalls_[oi]->GetTransform()->SetTranslaion(newPos);
		waterWeeds_[oi]->GetTransform()->SetTranslaion(newPos);
		grounds_[oi]->GetTransform()->SetTranslaion(Vector3(newPos.x, StageInformation::groundDepth_, 0.0f));
		waterSpaces_[oi]->SetTranslate({ newPos.x, 0.0f, 0.0f });

		mountains_[oi]->GetTransform()->SetTranslaion(newMountainPos);
		trees_[oi]->GetTransform()->SetTranslaion(newMountainPos);
		grass_[oi]->GetTransform()->SetTranslaion(newMountainPos);
		cloud_[oi]->GetTransform()->SetTranslaion(newMountainPos);
		cloud_[oi]->GetTransform()->SetTranslationZ(cloudZ);
		moai_[oi]->GetTransform()->SetTranslaion(newMountainPos);
		nico_[oi]->GetTransform()->SetTranslaion(newMountainPos);
		ufo_[oi]->GetTransform()->SetTranslaion(newMountainPos);
		ufosmoke_[oi]->GetTransform()->SetTranslaion(newMountainPos);
	}
	stageLoopCount_ = 0;
	mountainLoopCount_ = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void WorldObjects::Update(float playerPos) {
	playerPos_ = playerPos;
	for (uint32_t oi = 0; oi < kStageMax_; ++oi) {
		worldWalls_[oi]	->Update();
		waterWeeds_[oi]	->Update();
		grounds_[oi]	->Update();
		waterSpaces_[oi]->Update();
		mountains_[oi]	->Update();
		trees_[oi]		->Update();
		grass_[oi]		->Update();
		cloud_[oi]		->Update();
		moai_[oi]		->Update();
		nico_[oi]		->Update();
		ufo_[oi]		->Update();
		ufosmoke_[oi]	->Update();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void WorldObjects::Draw() const {
	Engine::SetPipeline(PipelineType::NormalPipeline);
	for (uint32_t oi = 0; oi < kStageMax_; ++oi) {
		worldWalls_[oi]->Draw();
		waterWeeds_[oi]->Draw();
		mountains_[oi]->Draw();

		trees_[oi]->Draw();
		grass_[oi]->Draw();
		moai_[oi]->Draw();
		nico_[oi]->Draw();
		ufo_[oi]->Draw();
		ufosmoke_[oi]->Draw();
			
	}
	
	Engine::SetPipeline(PipelineType::WaterLightingPipeline);
	for (const std::unique_ptr<Ground>& ground : grounds_) {
		ground->Draw();
	}

	//半透明なやつ
	Engine::SetPipeline(PipelineType::NormalPipeline);
	for(uint32_t oi = 0; oi < kStageMax_; ++oi) {
				
		cloud_[oi]->Draw();
	}
}

void WorldObjects::DrawWater() const {
	Engine::SetPipeline(PipelineType::NotCullingPipeline);
	// このクラスは一番最後に描画
	for (const std::unique_ptr<WaterSpace>& waterSpace : waterSpaces_) {
		waterSpace->Draw();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　るーぷ
//////////////////////////////////////////////////////////////////////////////////////////////////

void WorldObjects::LoopStage() {
	// playerが一定間隔進んだら新しいステージを生成する
	if (playerPos_ < 3000.0f) {
		return;
	}

	if (playerPos_ > (StageInformation::stageWidthEvery_ * (stageLoopCount_ + 1)) + 200.0f) {
		++stageLoopCount_;
		size_t index = static_cast<size_t>(nowStageIndex_);
		// 新しく設置する座標を求める
		Vector3 newPos = StageInformation::worldWallPos_;
		newPos.x += StageInformation::stageWidthEvery_ * (stageLoopCount_ + 1);

		worldWalls_[index]->GetTransform()->SetTranslaion(newPos);
		waterWeeds_[index]->GetTransform()->SetTranslaion(newPos);
		grounds_[index]->GetTransform()->SetTranslaion(Vector3(newPos.x, StageInformation::groundDepth_, 0.0f));
		waterSpaces_[index]->SetTranslate({ newPos.x, 0.0f, 0.0f });

		nowStageIndex_ = !nowStageIndex_;
	}

	// 8000の物
	if (playerPos_ > ((StageInformation::stageWidthEvery_ * 2.0f) * (mountainLoopCount_ + 1)) + 400.0f) {
		++mountainLoopCount_;
		size_t index = static_cast<size_t>(nowMountainIndex_);
		// 新しく設置する座標を求める
		Vector3 newPos = {0.0f, 0.0f, 0.0f};
		newPos.x += (StageInformation::stageWidthEvery_ * 2.0f) * (mountainLoopCount_ + 1);

		mountains_[index]->GetTransform()->SetTranslaion(newPos);
		trees_[index ]->GetTransform()->SetTranslaion(newPos);
		grass_[index ]->GetTransform()->SetTranslaion(newPos);
		moai_[index]->GetTransform()->SetTranslaion(newPos);
		nico_[index]->GetTransform()->SetTranslaion(newPos);
		ufo_[index]->GetTransform()->SetTranslaion(newPos);
		ufosmoke_[index]->GetTransform()->SetTranslaion(newPos);
		newPos.z = cloudZ;
		cloud_[index]->GetTransform()->SetTranslaion(newPos);

		nowMountainIndex_ = !nowMountainIndex_;
	}
}

void WorldObjects::CollisionPlayerToWaterSpace(Player* player) {
	for (auto& waterSpace : waterSpaces_) {

		// デカい距離で初期化
		float minLength = 999;

		// 水面との距離を求める
		for (size_t oi = 0; oi < waterSpace->GetWorldTopFaceList().size(); ++oi) {
			// playerのY座標と波の面のY座標との最短の距離を求める
			Vector3 distans = player->GetTransform()->GetTranslation() - waterSpace->GetWorldTopFaceList()[oi];
			distans.z = 0;
			float length = std::abs(distans.Length());
			if (length < minLength) {
				minLength = length;
			}
		}

		// 距離に応じた水との接触判定
		if (minLength < player->GetRadius()) {
			player->SetHitWaterSurface(true);
			break;
		} else {
			player->SetHitWaterSurface(false);
		}
	}
}
