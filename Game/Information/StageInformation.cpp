#include "StageInformation.h"
#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
#endif

std::vector<std::vector<std::string>> StageInformation::stageInformationArray_;
uint32_t StageInformation::nowStageNum_ = 0;
float StageInformation::stageWidth_ = 8000.0f;
float StageInformation::stageWidthEvery_ = 32.0f;
float StageInformation::groundDepth_ = -40.5f;

StageInformation::StageInformation() {
	stageInformationArray_.clear();
}

StageInformation::~StageInformation() {
}

void StageInformation::Init() {
	// ステージ1
	Add("aoi");
	Add("tubasa");
	Build();

	// ステージ2
	Add("tubasa");
	Build();

	buildArray_.clear();
}

void StageInformation::Update() {

}

void StageInformation::Add(const std::string& name) {
	buildArray_.push_back(name);
}

void StageInformation::Build() {
	stageInformationArray_.push_back(buildArray_);
	buildArray_.clear();
}

std::vector<std::string> StageInformation::GetStage() {
	return stageInformationArray_[nowStageNum_];
}

