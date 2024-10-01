#pragma once
#include "Engine/Assets/Mesh.h"
#include "Engine/Assets/Material.h"
#include "Engine/Assets/WorldTransform.h"
#include "Engine/Assets/ViewProjection.h"
#include "Engine/Assets/TransformationMatrix.h"
#include "Engine/Manager/TextureManager.h"

class Triangle {
public:

	Triangle();
	~Triangle();

	void Init(ID3D12Device* device, const Mesh::Vertices& vertex);

	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="commandList"></param>
	void Draw(ID3D12GraphicsCommandList* commandList, const WorldTransform& worldTransform, const ViewProjection* viewProjection);

#ifdef _DEBUG
	void ImGuiDraw(const std::string& name);
#endif

private:

	std::unique_ptr<Mesh> mesh_;
	std::unique_ptr<Material> material_;
	
};
