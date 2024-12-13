#pragma once
#include <nlohmann/json.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

using json = nlohmann::json;

class Score {

public:

	Score();
	~Score();

	void Init();
	void WriteFile(float count);
	void LoadJsonFile(float* time);

private:

	const std::string kFilePath_ = "./Game/Resources/GameData/Score/";
	const std::string fileName_ = "score.json";

	std::string filePath;

	std::vector<float> topScores_;

	std::string hiScore;
	std::string secondScore;
	std::string thirdScore;
};

