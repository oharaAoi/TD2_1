#pragma once
#include <list>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include "Game/GameObject/BasePlacementObject.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum class PlacementObjType {
	Test1_Type,
	Test2_Type,
};

/// <summary>
/// 障害物を配置するためのクラス
/// </summary>
class PlacementObjectEditer {
public:

	struct LoadData {
		Vector3 pos_;
		Vector3 scale_;
		Vector4 rotate_;
		float radius_;
		PlacementObjType type_;
		
		LoadData(const Vector3& scale, const Vector4& rotate, const Vector3& pos,
				 const float& radius, const PlacementObjType& type) {
			scale_ = scale, rotate_ = rotate, pos_ = pos,  radius_ = radius, type_ = type;
		}
	};

	struct Group {
		std::vector<LoadData> loadData_;
	};

	struct ObjectData {
		std::unique_ptr<BasePlacementObject> object_;
		PlacementObjType type_;
	};

	PlacementObjectEditer();
	~PlacementObjectEditer();

	void Init();
	void Load(const std::string& fileName);
	void LoadAllFile();

#ifdef _DEBUG

	void Update();
	void Draw() const;

	void Debug_Gui();
	void NewGroup_Config();
	void Edit_Config();

	void Save(const std::string& fileName, const std::list<ObjectData>& list);

	void Inport();

	std::string GetObjectString(const PlacementObjType& type);
	
private:

	std::list<ObjectData> debug_BasePlacementObj_;
	std::list<ObjectData> inport_BasePlacementObj_;
	// 新しく生成する種類
	PlacementObjType newPopType_;

	char exportFileName_[64];
	std::string inportFileName_;

	int inportIndex_;

#endif

	const std::string& kDirectoryPath_ = "./Game/Resources/GameData/PlacementObjectData/";
	std::map<std::string, Group> groupMap_;

	std::vector<std::string> fileNames_;

};


