#pragma once
#include <vector>
#include <list>
#include "Game/GameObject/BasePlacementObject.h"
#include "Engine/Math/MyRandom.h"
#include "Game/GameObject/Fish.h"
#include "Game/GameObject/Item.h"
#include "Game/Information/StageInformation.h"
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

	void SetObstacles(const std::vector<std::string>& stageInformation);
	void Inport(const std::string& fileName);

	void LoadAllFile();
	std::map<std::string, Group> LoadFile(const std::string& fileName);
	void MergeMaps(const std::map<std::string, Group>& map1);

	// Randomで配置する
	void RandomAddObject();

	void SetPlayerPosition(const Vector3& pos) { playerPos_ = pos; }

	std::list<std::unique_ptr<BasePlacementObject>>& GetPlacementObject() { return obstaclesList_; }
	const float GetUpdateLenght() const { return playerDrawLenght_; }

#ifdef _DEBUG
	void Debug_Gui();

private:

	std::string inportFileName_;
	int inportIndex_;

#endif // _DEBUG

private:

	// ファイルパス
	const std::string& kDirectoryPath_ = "./Game/Resources/GameData/PlacementObjectData/";
	// 障害物のリスト
	std::list<std::unique_ptr<BasePlacementObject>> obstaclesList_;
	// 障害物の位置が入っているリスト
	std::map<std::string, Group> groupMap_;
	// ファイル名が入っているリスト
	std::vector<std::string> fileNames_;

	Vector3 playerPos_;
	float playerDrawLenght_ = 200.0f;
};

