#pragma once
#include <vector>
#include <list>
#include "Game/GameObject/BasePlacementObject.h"
#include "Engine/Math/MyRandom.h"
#include "Game/GameObject/Fish.h"
#include "Game/GameObject/Item.h"
#include "Game/GameObject/Rock.h"
#include "Game/GameObject/Bird.h"
#include "Game/GameObject/Driftwood.h"
#include "Game/GameObject/Waterweed.h"
#include "Game/GameObject/Coin.h"
#include "Game/Information/StageInformation.h"
#include "Engine/Math/MyRandom.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/// <summary>
/// 障害物を管理するクラス
/// </summary>
class ObstaclesManager {

	friend class PlacementObjectEditer;

public:

	struct LoadData {
		Vector3 pos_;
		Vector3 scale_;
		Vector4 rotate_;
		float radius_;
		PlacementObjType type_;

		LoadData(const Vector3& scale, const Vector4& rotate, const Vector3& pos,
				 float radius, const PlacementObjType& type) {
			scale_ = scale, rotate_ = rotate, pos_ = pos, radius_ = radius, type_ = type;
		}
	};

	struct Group {
		std::vector<LoadData> loadData_;
		uint32_t level;
		float coolTime_= 5;
	};

	struct ObjectData {
		std::unique_ptr<BasePlacementObject> object_;
		PlacementObjType type_;
	};

public:

	ObstaclesManager();
	~ObstaclesManager();

	void Finalize();
	void Init();
	void Update();
	void Draw() const;

	void RandomImport();
	
	// ランダム配置の追加
	void RandomImportCreate();
	
	// 配置系
	void SetObstacles(const std::vector<std::string>& stageInformation);
	void Inport(const std::string& fileName, uint32_t level);

	// 読み込み系
	void LoadAllFile();
	std::map<std::string, Group> LoadFile(const std::string& fileName);
	void MergeMaps(const std::map<std::string, Group>& map1);

	// Randomで配置する
	void RandomAddObject();

	// アクセッサ
	void SetPlayerPosition(const Vector3& pos) { playerPos_ = pos; }

	std::list<std::unique_ptr<BasePlacementObject>>& GetPlacementObject() { return obstaclesList_; }
	const float GetUpdateLenght() const { return playerDrawLenght_; }

	const uint32_t GetMaxCoins() const { return coinNum_; }

#ifdef _DEBUG
	void Debug_Gui();

private:

	std::string inportFileName_;
	int inportIndex_;
	uint32_t debug_importLevel_ = 0;

#endif // _DEBUG

private:

	// ファイルパス
	const std::string& kDirectoryPath_ = "./Game/Resources/GameData/PlacementObjectData/";
	// 障害物のリスト
	std::list<std::unique_ptr<BasePlacementObject>> obstaclesList_;
	// 障害物の位置が入っているリスト
	std::map<int, std::map<std::string, Group>> groupMap_;
	// ファイル名が入っているリスト
	std::vector<std::string> fileNames_;
	// レベルごとでのファイル名
	std::map<int, std::vector<std::string>> levelFileName_;

	std::list<LoadData> randomImportArray_;

	// 現在のレベル
	uint32_t importLevel_;

	// playerに関する情報
	Vector3 playerPos_;
	Vector3 prePlayerPos_;
	// 描画をする距離(この距離以上離れているオブジェクトは描画しない)
	float playerDrawLenght_ = 360.0f;

	// coinの数
	uint32_t coinNum_ = 0;
};

