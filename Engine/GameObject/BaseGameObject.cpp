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
		Render::DrawModel(model_, transform_.get(), materials);
	} else {
		Engine::SetPipeline(PipelineKind::kSkinningPipeline);
		Render::DrawAnimationModel(model_, animetor_->GetSkinning(), transform_.get(), materials);
	}
}

void BaseGameObject::SetObject(const std::string& objName) {
	model_ = ModelManager::GetModel(objName);
	for (size_t oi = 0; oi < model_->GetMaterialsSize(); ++oi) {
		materials.push_back(Engine::CreateMaterial(model_->GetMaterialData(model_->GetMesh(oi)->GetUseMaterial())));
	}
}

void BaseGameObject::SetAnimater(const std::string& directoryPath, const std::string& objName) {
	animetor_ = std::make_unique<Animetor>();
	animetor_->LoadAnimation(directoryPath, objName, model_);
}

void BaseGameObject::SetColor(const Vector4& color) {
	for (size_t oi = 0; oi < materials.size(); ++oi) {
		materials[oi]->SetColor(color);
	}
}

#ifdef _DEBUG
void BaseGameObject::Debug_Gui() {
	transform_->Debug_Gui();
	model_->Debug_Gui("Test");
}
#endif // _DEBUG