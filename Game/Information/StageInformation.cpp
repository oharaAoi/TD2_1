#include "StageInformation.h"
#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
#endif

std::vector<std::vector<std::string>> StageInformation::stageInformationArray_;
uint32_t StageInformation::nowStageNum_ = 0;

StageInformation::StageInformation() {
	stageInformationArray_.clear();
}

StageInformation::~StageInformation() {
}

void StageInformation::Init() {
	Add("aoi");
	Add("tubasa");
	Build();

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

