#include "BaseGameObject.h"

void BaseGameObject::Finalize() {
}

void BaseGameObject::Init() {
	transform_ = Engine::CreateWorldTransform();
	animetor_ = nullptr;
}

void BaseGameObject::Update() {
	if (animetor_ != nullptr) {
		animetor_->Update();
	}
	transform_->Update();
}

void BaseGameObject::Draw() const {
	if (animetor_ == nullptr) {
		Engine::SetPipeline(PipelineKind::kNormalPipeline);
		Render::DrawModel(model_, transform_.get(), materialArray_);
	} else {
		Engine::SetPipeline(PipelineKind::kSkinningPipeline);
		Render::DrawAnimationModel(model_, animetor_->GetSkinning(), transform_.get());
	}
}

void BaseGameObject::SetObject(const std::string& objName) {
	model_ = ModelManager::GetModel(objName);
	materialArray_ = model_->copyMaterialArray();
}

void BaseGameObject::SetAnimater(const std::string& directoryPath, const std::string& objName) {
	animetor_ = std::make_unique<Animetor>();
	animetor_->LoadAnimation(directoryPath, objName, model_);
}

void BaseGameObject::SetColor(const Vector4& color) {
	for (const auto& pair : this->materialArray_) {
		pair.second->SetColor(color);
	}
}

#ifdef _DEBUG
void BaseGameObject::Debug_Gui() {
	transform_->Debug_Gui();
	model_->Debug_Gui("Test");
}
#endif // _DEBUG