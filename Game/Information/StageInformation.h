#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/// <summary>
/// Stage情報をまとめたクラス
/// </summary>
class StageInformation {
public:

	StageInformation();
	~StageInformation();

	void Init();
	void Update();

	void Add(const std::string& name);

	void Build();

	static std::vector<std::string> GetStage();
	static uint32_t GetStageNumMax() { return static_cast<uint32_t>(stageInformationArray_.size()); }

	static uint32_t GetNowStage() { return nowStageNum_; }
	static void SetNowStage(uint32_t num) { nowStageNum_ = num; }

private:
	std::string  kDirectoryPath_ = "./Game/Resources/GameData/StageInformation/stage";
	static std::vector<std::vector<std::string>> stageInformationArray_;
	static uint32_t nowStageNum_;

	std::vector<std::string> buildArray_;
};

