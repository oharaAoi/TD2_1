#include "Material.h"

Material::Material() {
}

Material::~Material() {
	Finalize();
}

void Material::Finalize() {
	materialBuffer_.Reset();
}

void Material::Init(ID3D12Device* device) {
	// ---------------------------------------------------------------
	// ↓Materialの設定
	// ---------------------------------------------------------------
	materialBuffer_ = CreateBufferResource(device, sizeof(PBRMaterial));
	materialBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&material_));
	// 色を決める
	material_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	material_->enableLighting = false;
	material_->uvTransform = MakeIdentity4x4();
	material_->shininess = 0;

	material_->diffuseColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	material_->specularColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	material_->roughness = 0.5f;
	material_->metallic = 0.5f;

	uvTranslation_ = { 0,0,0 };
	uvScale_ = { 1,1,1 };
	uvRotation_ = { 0,0,0 };
}

void Material::Update(const Matrix4x4& uvTransform) {
	material_->uvTransform = uvTransform;
}

void Material::Draw(ID3D12GraphicsCommandList* commandList) {
	commandList->SetGraphicsRootConstantBufferView(0, materialBuffer_->GetGPUVirtualAddress());
}

#ifdef _DEBUG
void Material::ImGuiDraw() {
	ImGui::DragFloat2("uvTranslate", &uvTranslation_.x, 0.01f, -10.0f, 10.0f);
	ImGui::DragFloat2("uvScale", &uvScale_.x, 0.01f, -10.0f, 10.0f);
	ImGui::SliderAngle("UvRotate", &uvRotation_.z);
	ImGui::ColorEdit3("color", &material_->color.x);
	ImGui::Combo("Lighting", &material_->enableLighting, "None\0Lambert\0HalfLambert");

	material_->uvTransform = MakeAffineMatrix(kTransform(uvScale_, uvRotation_, uvTranslation_));
}
#endif

void Material::SetMaterialData(ModelMaterialData materialData) {
	materialData_ = materialData;

	material_->color = materialData_.albedo;
	material_->enableLighting = true;
	material_->uvTransform = MakeIdentity4x4();
	// 反射用素
	material_->diffuseColor = materialData_.diffuse;
	material_->specularColor = materialData_.specular;

	material_->roughness = 0.5f;
	material_->metallic = 0.5f;
	material_->shininess = 0;

	material_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	material_->enableLighting = true;
	material_->uvTransform = MakeIdentity4x4();
	material_->shininess = 50;

	material_->diffuseColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	material_->specularColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	material_->roughness = 0.5f;
	material_->metallic = 0.5f;
}