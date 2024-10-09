#include "MeshManager.h"

std::unordered_map<std::string, std::unique_ptr<Mesh>> MeshManager::meshMap_;
std::vector<std::string> MeshManager::meshNameList_;

MeshManager::~MeshManager() {
}

MeshManager* MeshManager::GetInstance() {
	static MeshManager instance;
	return &instance;
}

void MeshManager::Init() {

}

void MeshManager::Finalize() {
	meshMap_.clear();
}

void MeshManager::AddMesh(ID3D12Device* device, const std::string& modelName, const std::vector<Mesh::VertexData> vertexData, std::vector<uint32_t>& indices) {
	// Meshクラスの宣言
	std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>();
	mesh->Init(device, vertexData, indices);
	// Meshを配列に格納
	meshMap_[modelName] = std::move(mesh);
	meshNameList_.push_back(modelName);
}

Mesh* MeshManager::GetMesh(const std::string& meshName) {
	if (auto it = meshMap_.find(meshName); it == meshMap_.end()) {
		assert("not find Model!");
	}

	meshNameList_.push_back(meshName);
	return meshMap_[meshName].get();
}
