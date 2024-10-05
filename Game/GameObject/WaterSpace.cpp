#include "WaterSpace.h"

WaterSpace::WaterSpace() {}
WaterSpace::~WaterSpace() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void WaterSpace::Init(const std::string& directorPath, const std::string& fileName) {
	materialArray_ = LoadMaterialData(directorPath, fileName, Engine::GetDevice());
	meshArray_ = LoadVertexData(directorPath, fileName, Engine::GetDevice());
	transform_ = Engine::CreateWorldTransform();

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
	waveParameter_->amplitude = 0.2f;
	waveParameter_->frequency = 1.0f;

	for (auto& material : materialArray_) {
		material.second->SetColor({1.0f, 1.0f, 1.0f, 0.5f});
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void WaterSpace::Update() {
	waveParameter_->time += GameTimer::DeltaTime();
	transform_->Update();
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