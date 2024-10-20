#include "Animetor.h"
#include "Engine/Assets/AnimetionClip.h"
#include "Engine/Assets/Skeleton.h"
#include "Engine/Assets/Skinning.h"

Animetor::Animetor() {}
Animetor::~Animetor() {}

 void Animetor::Finalize() {
	skinning_->Finalize();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Animetor::Update() {
	animetionClip_->Update();
	animetionClip_->ApplyAnimation(skeleton_.get());
	skeleton_->Update();
	skinning_->Update(skeleton_.get());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　ファイル読み込み
//////////////////////////////////////////////////////////////////////////////////////////////////

void Animetor::LoadAnimation(const std::string& directoryPath, const std::string& fileName, Model* model) {
	animetionClip_ = std::make_unique<AnimetionClip>();
	animetionClip_->LoadAnimation(directoryPath, fileName, model->GetRootNodeName());
	skeleton_ = std::make_unique<Skeleton>();
	skeleton_->CreateSkeleton(model->GetNode());
	skeleton_->Init();
	skinning_ = Engine::CreateSkinning(skeleton_.get(), model, 0);

	animetionClip_->Update();
	animetionClip_->ApplyAnimation(skeleton_.get());
}

const bool Animetor::GetIsAnimationFinish() const {
	return animetionClip_->GetIsAnimationFinish();
}
