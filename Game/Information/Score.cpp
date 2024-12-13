#include "Score.h"

Score::Score() {}
Score::~Score() {}

void Score::Init() {
	if (!std::filesystem::exists(kFilePath_)) {
		std::filesystem::create_directories(kFilePath_);
	}

	hiScore = "hiScore";
	secondScore = "secondScore";
	thirdScore = "thirdScore";

	// スコアがなかったら新しく生成する用
	json scoreData;
	std::ifstream inputFile(kFilePath_ + fileName_);
	if (inputFile.is_open()) {
		inputFile >> scoreData;
		inputFile.close();
	} else {
		std::string id = "score1";
		
		std::ofstream outputFile(kFilePath_ + fileName_);
		if (outputFile.is_open()) {
			outputFile << std::setw(4) << scoreData << std::endl;
			outputFile.close();
		} else {
			std::cerr << "ファイルを開けませんでした。" << std::endl;
		}
	}
}

void Score::WriteFile(float count) {
	json scoreData;

	std::ifstream inputFile(kFilePath_ + fileName_);
	if (inputFile.is_open()) {
		inputFile >> scoreData;
		inputFile.close();
	}

	std::string name = "score";
	std::string id = name + std::to_string(scoreData.size() + 1);
	scoreData[id] = count;

	// ファイルに更新されたJSONデータを書き込む
	std::ofstream outputFile(kFilePath_ + fileName_);
	if (outputFile.is_open()) {
		outputFile << std::setw(4) << scoreData << std::endl;
		outputFile.close();
	} else {
		std::cerr << "ファイルを開けませんでした。" << std::endl;
	}
}

void Score::LoadJsonFile(float* time) {
	json scoreData;

	std::ifstream inputFile(kFilePath_ + fileName_);
	if (inputFile.is_open()) {
		inputFile >> scoreData;
		inputFile.close();
	}

	std::vector<float> scores;
	for (uint32_t oi = 0; oi < scoreData.size(); ++oi) {
		std::string name = "score";
		std::string id = name + std::to_string(oi + 1);
		scores.push_back(scoreData[id]);
	}

	// 降順にソート
	std::sort(scores.begin(), scores.end(), [](float a, float b) {
		return a > b; // 大きい順
			  });

	// 上位3つを取得
	topScores_.clear();
	size_t topCount = std::min(size_t(3), scores.size());
	std::vector<float> topScores(scores.begin(), scores.begin() + topCount);
	topScores_ = std::move(topScores);

	for (uint32_t oi = 0; oi < 3; ++oi) {
		if (topScores_.size() <= oi) {
			time[oi] = 0;
		} else {
			time[oi] = topScores_[oi];
		}
	}
}
