#include "Sprite.h"
#include "Render.h"

Sprite::Sprite() {}
Sprite::~Sprite() {
	vertexBuffer_.Reset();
	indexBuffer_.Reset();
	materialBuffer_.Reset();
	transformBuffer_.Reset();
}

void Sprite::Init(ID3D12Device* device, const std::string& fileName) {
	textureSize_ = TextureManager::GetInstance()->GetTextureSize(fileName);
	textureName_ = fileName;
	rectRange_ = textureSize_;
	leftTop_ = { 0.0f, 0.0f };
	pivot_ = { 0.5f, 0.5f };

	// ----------------------------------------------------------------------------------
	vertexBuffer_ = CreateBufferResource(device, sizeof(TextureMesh) * 4);
	// リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexBuffer_->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(TextureMesh) * 4;
	// 1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(TextureMesh);
	// Resourceにデータを書き込む 
	vertexData_ = nullptr;
	// アドレスを取得
	vertexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	Vector3 pivotOffset = {
		textureSize_.x * pivot_.x,
		textureSize_.y * pivot_.y,
		0.0f
	};

	// スプライトのサイズを基に頂点を設定
	Mesh::RectVetices rect = {
		{-pivotOffset.x, -pivotOffset.y, 0.0f, 1.0f},
		{textureSize_.x - pivotOffset.x, 0.0f - pivotOffset.y, 0.0f, 1.0f},
		{0.0f - pivotOffset.x, textureSize_.y - pivotOffset.y, 0.0f , 1.0f},
		{textureSize_.x - pivotOffset.x, textureSize_.y - pivotOffset.y , 0.0f, 1.0f},
	};

	/*Mesh::RectVetices rect = {
		{-(textureSize_.x * 0.5f) + pivotOffset.x, -(textureSize_.y * 0.5f) + pivotOffset.y, 0.0f, 1.0f},
		{+(textureSize_.x * 0.5f) + pivotOffset.x, -(textureSize_.y * 0.5f) + pivotOffset.y, 0.0f, 1.0f},
		{-(textureSize_.x * 0.5f) + pivotOffset.x, +(textureSize_.y * 0.5f) + pivotOffset.y, 0.0f, 1.0f},
		{+(textureSize_.x * 0.5f) + pivotOffset.x, +(textureSize_.y * 0.5f) + pivotOffset.y, 0.0f, 1.0f},
	};*/

	vertexData_[0].pos = rect.leftBottom;
	vertexData_[0].texcoord = { 0.0f, 1.0f }; // 左下
	vertexData_[1].pos = rect.leftTop;
	vertexData_[1].texcoord = { 0.0f, 0.0f }; // 左上
	vertexData_[2].pos = rect.rightBottom; // 右下
	vertexData_[2].texcoord = { 1.0f, 1.0f };
	vertexData_[3].pos = rect.rightTop;		// 右上
	vertexData_[3].texcoord = { 1.0f, 0.0f };

	// ----------------------------------------------------------------------------------
	indexBuffer_ = CreateBufferResource(device, sizeof(uint32_t) * 6);
	indexBufferView_.BufferLocation = indexBuffer_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * 6);
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;
	indexData_ = nullptr;
	indexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

	indexData_[0] = 0;
	indexData_[1] = 1;
	indexData_[2] = 2;

	indexData_[3] = 1;
	indexData_[4] = 3;
	indexData_[5] = 2;
	// ----------------------------------------------------------------------------------
	materialBuffer_ = CreateBufferResource(device, sizeof(TextureMaterial));
	materialBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	materialData_->uvTransform = MakeIdentity4x4();

	// ----------------------------------------------------------------------------------
	transformBuffer_ = CreateBufferResource(device, sizeof(TextureTransformData));
	transformBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&transformData_));

	transform_ = { {1.0f,1.0f,1.0f} , {0.0f, 0.0f, 0.0f}, {0, 0, 0} };
	uvTransform_ = { {1.0f,1.0f,1.0f} , {0.0f, 0.0f, 0.0f}, {0, 0, 0} };

	transformData_->wvp = Matrix4x4(
		MakeAffineMatrix(transform_)
		* MakeIdentity4x4()
		* MakeOrthograhicMatrix(0.0f, 0.0f, float(1280), float(720), 0.0f, 100.0f)
	);
}

void Sprite::Update() {
	materialData_->uvTransform = MakeAffineMatrix(uvTransform_);

	materialData_->uvTransform.m[0][0] = rectRange_.x / textureSize_.x;	// Xスケーリング
	materialData_->uvTransform.m[1][1] = rectRange_.y / textureSize_.y;	// Yスケーリング
	materialData_->uvTransform.m[3][0] = leftTop_.x / textureSize_.x;	// Xオフセット
	materialData_->uvTransform.m[3][1] = leftTop_.y / textureSize_.y;	// Yオフセット
	
	/*float uvLeft = leftTop_.x / textureSize_.x;
	float uvRight = (leftTop_.x + rectRange_.x) / textureSize_.x;
	float uvTop = leftTop_.y / textureSize_.y;
	float uvBottom = (leftTop_.y + rectRange_.y) / textureSize_.y;

	vertexData_[0].texcoord = { uvLeft, uvBottom };
	vertexData_[1].texcoord = { uvLeft, uvTop };
	vertexData_[2].texcoord = { uvRight, uvBottom };
	vertexData_[3].texcoord = { uvRight, uvTop };*/

	//Vector2 pivotOffset = {
	//	(pivot_.x - 0.5f) * textureSize_.x,  // ピボットオフセット（中心からのオフセット）
	//	(pivot_.y - 0.5f) * textureSize_.y   // ピボットオフセット（中心からのオフセット）
	//};

	//// アフィン変換行列の作成
	//Matrix4x4 affineMatrix = MakeAffineMatrix(transform_);
	//// テクスチャ位置を保持するための補正行列
	//Matrix4x4 correctionTranslation = MakeTranslateMatrix({ pivotOffset.x, pivotOffset.y, 0.0f });

	//// 最終的なスプライトの変換行列
	//transformData_->wvp = Matrix4x4(
	//	affineMatrix *  // ピボットによる変位を元に戻す
	//	correctionTranslation *
	//	MakeOrthograhicMatrix(0.0f, 0.0f, float(1280), float(720), 0.0f, 100.0f)
	//);
}

void Sprite::Draw(bool isBackGround) {
	Vector2 pivotOffset = {
		(pivot_.x - 0.5f) * textureSize_.x,  // ピボットオフセット（中心からのオフセット）
		(pivot_.y - 0.5f) * textureSize_.y   // ピボットオフセット（中心からのオフセット）
	};

	Matrix4x4 projection = Render::GetProjection2D();
	if (isBackGround) {
		transform_.translate.z = Render::GetFarClip();
	}
	// アフィン変換行列の作成
	Matrix4x4 affineMatrix = MakeAffineMatrix(transform_);
	// テクスチャ位置を保持するための補正行列
	Matrix4x4 correctionTranslation = MakeTranslateMatrix({ pivotOffset.x, pivotOffset.y, 0.0f });

	// 最終的なスプライトの変換行列
	transformData_->wvp = Matrix4x4(
		affineMatrix *  // ピボットによる変位を元に戻す
		correctionTranslation *
		projection
	);

	Render::DrawSprite(this);
}

void Sprite::PostDraw(ID3D12GraphicsCommandList* commandList) const {
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
	commandList->IASetIndexBuffer(&indexBufferView_);
	commandList->SetGraphicsRootConstantBufferView(0, materialBuffer_->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(1, transformBuffer_->GetGPUVirtualAddress());
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList, textureName_, 2);
	commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}


void Sprite::SetTexture(const std::string& fileName) {
	textureName_ = fileName;
	textureSize_ = TextureManager::GetInstance()->GetTextureSize(fileName);
	rectRange_ = textureSize_;
	leftTop_ = { 0.0f, 0.0f };

	Vector3 pivotOffset = {
		textureSize_.x * pivot_.x,
		textureSize_.y * pivot_.y,
		0.0f
	};

	// スプライトのサイズを基に頂点を設定
	Mesh::RectVetices rect = {
		{-pivotOffset.x, -pivotOffset.y, 0.0f, 1.0f},
		{textureSize_.x - pivotOffset.x, 0.0f - pivotOffset.y, 0.0f, 1.0f},
		{0.0f - pivotOffset.x, textureSize_.y - pivotOffset.y, 0.0f , 1.0f},
		{textureSize_.x - pivotOffset.x, textureSize_.y - pivotOffset.y , 0.0f, 1.0f},
	};

	vertexData_[0].pos = rect.leftBottom;
	vertexData_[0].texcoord = { 0.0f, 1.0f }; // 左下
	vertexData_[1].pos = rect.leftTop;
	vertexData_[1].texcoord = { 0.0f, 0.0f }; // 左上
	vertexData_[2].pos = rect.rightBottom; // 右下
	vertexData_[2].texcoord = { 1.0f, 1.0f };
	vertexData_[3].pos = rect.rightTop;		// 右上
	vertexData_[3].texcoord = { 1.0f, 0.0f };

}

void Sprite::SetTextureCenterPos(const Vector2& centerPos) {
	transform_.translate.x = centerPos.x;
	transform_.translate.y = centerPos.y;
	transform_.translate.z = 0;
}

void Sprite::SetTextureSize(const Vector2& size) {
	Mesh::RectVetices rect = {
		{-(size.x / 2.0f), -(size.y / 2.0f), 0.0f, 1.0f},
		{+(size.x / 2.0f), -(size.y / 2.0f), 0.0f, 1.0f},
		{-(size.x / 2.0f), +(size.y / 2.0f), 0.0f, 1.0f},
		{+(size.x / 2.0f), +(size.y / 2.0f), 0.0f, 1.0f},
	};

	vertexData_[0].pos = rect.leftBottom;
	vertexData_[0].texcoord = { 0.0f, 1.0f }; // 左下
	vertexData_[1].pos = rect.leftTop;
	vertexData_[1].texcoord = { 0.0f, 0.0f }; // 左上
	vertexData_[2].pos = rect.rightBottom; // 右下
	vertexData_[2].texcoord = { 1.0f, 1.0f };
	vertexData_[3].pos = rect.rightTop;		// 右上
	vertexData_[3].texcoord = { 1.0f, 0.0f };
}

void Sprite::SetPivot(const Vector2& pivot) {
	pivot_ = pivot;
	Vector3 pivotOffset = {
	textureSize_.x * pivot_.x,
	textureSize_.y * pivot_.y,
	0.0f
	};

	// スプライトのサイズを基に頂点を設定
	Mesh::RectVetices rect = {
		{-pivotOffset.x, -pivotOffset.y, 0.0f, 1.0f},
		{textureSize_.x - pivotOffset.x, 0.0f - pivotOffset.y, 0.0f, 1.0f},
		{0.0f - pivotOffset.x, textureSize_.y - pivotOffset.y, 0.0f , 1.0f},
		{textureSize_.x - pivotOffset.x, textureSize_.y - pivotOffset.y , 0.0f, 1.0f},
	};

	vertexData_[0].pos = rect.leftBottom;
	vertexData_[0].texcoord = { 0.0f, 1.0f }; // 左下
	vertexData_[1].pos = rect.leftTop;
	vertexData_[1].texcoord = { 0.0f, 0.0f }; // 左上
	vertexData_[2].pos = rect.rightBottom; // 右下
	vertexData_[2].texcoord = { 1.0f, 1.0f };
	vertexData_[3].pos = rect.rightTop;		// 右上
	vertexData_[3].texcoord = { 1.0f, 0.0f };
}

#ifdef _DEBUG
void Sprite::Debug_Gui() {
	ImGui::DragFloat3("translation", &transform_.translate.x, 2.0f);
	ImGui::DragFloat2("scale", &transform_.scale.x, 0.01f, -10.0f, 10.0f);
	ImGui::SliderAngle("rotation", &transform_.rotate.z);
	ImGui::DragFloat2("pivot", &pivot_.x, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat2("textureSize", &textureSize_.x, 1.0f);
	ImGui::DragFloat2("rectRange", &rectRange_.x, 1.0f);
	ImGui::DragFloat2("leftTop", &leftTop_.x, 1.0f);

	Vector3 pivotOffset = {
	textureSize_.x * pivot_.x,
	textureSize_.y * pivot_.y,
	0.0f
	};

	// スプライトのサイズを基に頂点を設定
	Mesh::RectVetices rect = {
		{-pivotOffset.x, -pivotOffset.y, 0.0f, 1.0f},
		{textureSize_.x - pivotOffset.x, 0.0f - pivotOffset.y, 0.0f, 1.0f},
		{0.0f - pivotOffset.x, textureSize_.y - pivotOffset.y, 0.0f , 1.0f},
		{textureSize_.x - pivotOffset.x, textureSize_.y - pivotOffset.y , 0.0f, 1.0f},
	};

	vertexData_[0].pos = rect.leftBottom;
	vertexData_[0].texcoord = { 0.0f, 1.0f }; // 左下
	vertexData_[1].pos = rect.leftTop;
	vertexData_[1].texcoord = { 0.0f, 0.0f }; // 左上
	vertexData_[2].pos = rect.rightBottom; // 右下
	vertexData_[2].texcoord = { 1.0f, 1.0f };
	vertexData_[3].pos = rect.rightTop;		// 右上
	vertexData_[3].texcoord = { 1.0f, 0.0f };
	
	if (ImGui::TreeNode("material")) {
		ImGui::DragFloat2("uvTranslation", &uvTransform_.translate.x, 0.1f);
		ImGui::DragFloat2("uvScale", &uvTransform_.scale.x, 0.01f, -10.0f, 10.0f);
		ImGui::SliderAngle("uvRotation", &uvTransform_.rotate.z);
		ImGui::TreePop();
	}
}
#endif