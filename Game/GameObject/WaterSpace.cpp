#include "WaterSpace.h"

WaterSpace::WaterSpace() {}
WaterSpace::~WaterSpace() {}

void WaterSpace::Finalize() {
	waveParameter_ = nullptr;
	waveParameterBuffer_.Reset();
	transform_->Finalize();
	meshArray_.clear();
	materialArray_.clear();

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void WaterSpace::Init(const std::string& directorPath, const std::string& fileName) {
	// -------------------------------------------------
	// ↓ 基本のGPUに送る情報を初期化する
	// -------------------------------------------------
	materialArray_ = LoadMaterialData(directorPath, fileName, Engine::GetDevice());
	meshArray_ = LoadVertexData(directorPath, fileName, Engine::GetDevice());
	transform_ = Engine::CreateWorldTransform();

	// -------------------------------------------------
	// ↓ GPUに送る波の情報
	// -------------------------------------------------
	waveParameterBuffer_ = CreateBufferResource(Engine::GetDevice(), sizeof(WaveParameter));
	// リソースの先頭のアドレスから使う
	waveParameteBufferView_.BufferLocation = waveParameterBuffer_->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点3つ分のサイズ
	waveParameteBufferView_.SizeInBytes = (UINT)sizeof(WaveParameter);
	// 1頂点当たりのサイズ
	waveParameteBufferView_.StrideInBytes = sizeof(WaveParameter);
	// Resourceにデータを書き込む 
	waveParameter_ = nullptr;
	// アドレスを取得
	waveParameterBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&waveParameter_));
	waveParameter_->time = 0;
	waveParameter_->amplitude = 0.4f;
	waveParameter_->frequency = 1.0f;

	// -------------------------------------------------
	// ↓ Materialの色を設定する
	// -------------------------------------------------
	for (auto& material : materialArray_) {
		material.second->SetColor({1.0f, 1.0f, 1.0f, 0.5f});
	}

	// -------------------------------------------------
	// ↓ MeshからBoxの上面だけの頂点を取得する
	// -------------------------------------------------
	float minFrontZ = 999;
	std::vector<Vector3> topFaceList;
	for (size_t i = 0; i < meshArray_[0]->GetIndexNum(); ++i) {
		if (meshArray_[0]->GetVertexData()[i].normal.y == 1.0f) {
			Vector4 position = meshArray_[0]->GetVertexData()[i].pos;
			topFaceList.push_back({ position.x, position.y, position.z });
			if (minFrontZ > position.z) {
				minFrontZ = position.z;
			}
		}
	}

	// Y軸だけ分かれば最悪いいので一番手前のものだけ取得
	for (size_t oi = 0; oi < topFaceList.size(); ++oi) {
		if (minFrontZ == topFaceList[oi].z) {
			topFaceList_.push_back(topFaceList[oi]);
		}
	}
	worldTopFaceList_.resize(topFaceList_.size());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void WaterSpace::Update() {
	waveParameter_->time += GameTimer::DeltaTime();
	transform_->Update();

	// 上面のworld座標を求めておく
	for (size_t oi = 0; oi < worldTopFaceList_.size(); ++oi) {
		worldTopFaceList_[oi] = Transform(topFaceList_[oi], transform_->GetWorldMatrix());
		worldTopFaceList_[oi].y += waveParameter_->amplitude * std::sinf(waveParameter_->frequency * (worldTopFaceList_[oi].x + waveParameter_->time));
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void WaterSpace::Draw() const {
	Render::DrawLightGroup(5);
	ID3D12GraphicsCommandList* commandList = Engine::GetCommandList();
	for (uint32_t oi = 0; oi < meshArray_.size(); oi++) {
		meshArray_[oi]->Draw(commandList);
		materialArray_.at(meshArray_[oi]->GetUseMaterial())->Draw(commandList);

		transform_->Draw(commandList);
		Render::GetViewProjection()->Draw(commandList);

		std::string textureName = materialArray_.at(meshArray_[oi]->GetUseMaterial())->GetMateriaData().textureFilePath;
		TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList, textureName, 3);

		commandList->SetGraphicsRootConstantBufferView(4, waveParameterBuffer_->GetGPUVirtualAddress());

		commandList->DrawIndexedInstanced(meshArray_[oi]->GetIndexNum(), 1, 0, 0, 0);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void WaterSpace::Debug_Gui() {
	ImGui::Begin("WaterSpace");
	ImGui::DragFloat("amplitude", &waveParameter_->amplitude, 0.1f);
	ImGui::DragFloat("frequency", &waveParameter_->frequency, 0.1f);
	ImGui::DragFloat("time", &waveParameter_->time, 0.1f);
	ImGui::End();
}
#endif // _DEBUG