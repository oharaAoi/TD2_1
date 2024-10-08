#include "WorldTransform.h"
#include <Math/MyMatrix.h>

WorldTransform::WorldTransform() {}
WorldTransform::~WorldTransform() {
	Finalize();
}

void WorldTransform::Init(ID3D12Device* device) {
	cBuffer_ = CreateBufferResource(device, sizeof(WorldTransformData));
	// データをマップ
	cBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&data_));

	// 値を初期化しておく
	scale_ = {1.0f, 1.0f, 1.0f};
	rotation_ = Quaternion();
	translation_ = { 0.0f, 0.0f, 0.0f };
	worldMat_ = MakeIdentity4x4();
}

void WorldTransform::Update(const Matrix4x4& mat) {
	rotation_ = (rotation_ * quaternion_).Normalize();
	quaternion_ = Quaternion();
	worldMat_ = mat * MakeAffineMatrix(scale_, rotation_, translation_);
	if (parentMat_ != nullptr) {
		worldMat_ *= *parentMat_;
	}
	// GPUに送るデータを更新
	data_->matWorld = worldMat_;
	data_->worldInverseTranspose = Transpose(Inverse(data_->matWorld));
}

void WorldTransform::Draw(ID3D12GraphicsCommandList* commandList) const {
	commandList->SetGraphicsRootConstantBufferView(1, cBuffer_->GetGPUVirtualAddress());
}

#ifdef _DEBUG
void WorldTransform::Debug_Gui() {
	ImGui::DragFloat3("scale", &scale_.x, 0.1f);
	ImGui::DragFloat4("rotation", &rotation_.x, 0.1f);
	ImGui::DragFloat4("quaternion", &quaternion_.x, 0.1f);
	ImGui::DragFloat3("translation", &translation_.x, 0.1f);
}
#endif

void WorldTransform::Finalize() {
	cBuffer_.Reset();
}

void WorldTransform::SetParent(const Matrix4x4& parentMat) {
	parentMat_ = &parentMat;
}

void WorldTransform::SetMatrix(const Matrix4x4& mat) {
	data_->matWorld = mat;
	data_->worldInverseTranspose = Transpose(Inverse(data_->matWorld));
}
