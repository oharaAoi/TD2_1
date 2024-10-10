#pragma once
#include <list>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include "Game/GameObject/BasePlacementObject.h"
#include "Game/Manager/ObstaclesManager.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;


/// <summary>
/// 障害物を配置するためのクラス
/// </summary>
class PlacementObjectEditer {
public:


	PlacementObjectEditer();
	~PlacementObjectEditer();

	void Init(ObstaclesManager* obstaclesManager);
	void LoadAllFile();
	void MergeMaps(const std::map<std::string, ObstaclesManager::Group>& map1);

#ifdef _DEBUG

	void Update();
	void Draw() const;

	void Debug_Gui();
	void NewGroup_Config();
	void Edit_Config();

	void Save(const std::string& fileName, const std::list<ObstaclesManager::ObjectData>& list);

	void Inport();

	std::string GetObjectString(const PlacementObjType& type);
	
private:

	std::list<ObstaclesManager::ObjectData> debug_BasePlacementObj_;
	std::list<ObstaclesManager::ObjectData> inport_BasePlacementObj_;
	// 新しく生成する種類
	PlacementObjType newPopType_;

	char exportFileName_[64];
	std::string inportFileName_;

	int inportIndex_;

#endif

	ObstaclesManager* obstaclesManager_ = nullptr;

	const std::string& kDirectoryPath_ = "./Game/Resources/GameData/PlacementObjectData/";
	std::map<std::string, ObstaclesManager::Group> groupMap_;

	std::vector<std::string> fileNames_;

};


