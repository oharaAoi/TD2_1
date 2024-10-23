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
		if (!scoreData.contains(hiScore)) {
			scoreData[hiScore] = { {"score", 0} };
		}

		if (!scoreData.contains(secondScore)) {
			scoreData[secondScore] = { {"score", 0} };
		}

		if (!scoreData.contains(thirdScore)) {
			scoreData[thirdScore] = { {"score", 0} };
		}

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

	///タイムの比較
	if (scoreData[hiScore]["score"] < count) {
		scoreData[hiScore]["score"] = count;

	} else if (scoreData[secondScore]["score"] < count) {
		scoreData[secondScore]["score"] = count;

	} else if (scoreData[thirdScore]["score"] < count) {
		scoreData[thirdScore]["score"] = count;
	}

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

	time[0] = scoreData[hiScore]["score"];
	time[1] = scoreData[secondScore]["score"];
	time[2] = scoreData[thirdScore]["score"];
}
