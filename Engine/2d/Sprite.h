#pragma once
#include "Engine/Math/Vector2.h"
#include "Engine/Assets/Mesh.h"
#include "Engine/Lib/Transform.h"
#include "Engine/Manager/TextureManager.h"
#include "Engine/Manager/ImGuiManager.h"
#include "Engine/Assets/WorldTransform.h"
#include "Engine/Assets/ViewProjection.h"

class Render;

class Sprite {
public:

	struct TextureMesh {
		Vector4 pos;
		Vector2 texcoord;
		float padding[2];
	};

	struct TextureMaterial {
		Vector4 color;
		Matrix4x4 uvTransform;
	};

	struct TextureTransformData {
		Matrix4x4 wvp;
	};

public:

	Sprite();
	~Sprite();

	void Init(ID3D12Device* device, const std::string& fileName);
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="rectRange">: 描画する範囲</param>
	/// <param name="leftTop">: 左上座標</param>
	void Update();
	void Draw();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="commandList"></param>
	void Draw(ID3D12GraphicsCommandList* commandList) const;

#ifdef _DEBUG
	void Debug_Gui();
#endif

public:

	/// <summary>
	/// Textureを設定し直す
	/// </summary>
	/// <param name="fileName">: Textureの名前</param>
	void SetTexture(const std::string& fileName);
	
	/// <summary>
	/// Textureの中心の位置を設定する
	/// </summary>
	/// <param name="centerPos">: newPosition</param>
	void SetTextureCenterPos(const Vector2& centerPos);

	/// <summary>
	/// 元となるTextureの大きさを設定する
	/// </summary>
	/// <param name="size">: Textureのsizeを設定する</param>
	void SetTextureSize(const Vector2& size);
	
	/// <summary>
	/// 描画するSpriteのsizeを変更する
	/// </summary>
	/// <param name="size">: sizeの大きさで描画される</param>
	void SetDrawTextureSize(const Vector2& size);

	/// <summary>
	/// 描画する範囲を設定する
	/// </summary>
	/// <param name="rectRange">: uv座標系上の描画する範囲</param>
	void SetRectRange(const Vector2& rectRange) { rectRange_ = rectRange; };

	/// <summary>
	/// Textureの左上を設定する
	/// </summary>
	/// <param name="leftTop">: 左上の位置(Textureのuv座標系上の位置)</param>
	void SetLeftTop(const Vector2& leftTop) { leftTop_ = leftTop; }
	
	/// <summary>
	/// Textureの中心位置を変更する
	/// </summary>
	/// <param name="pivot">: 0~1の範囲で設定</param>
	void SetPivot(const Vector2& pivot);

	const Vector2 GetCenterPos() const { return Vector2{ transform_.translate.x, transform_.translate.y}; }
	const Vector2 GetScale() const { return Vector2(transform_.scale.x, transform_.scale.y); }
	const float GetRotate() const { return transform_.rotate.z; }

	void SetCenterPos(const Vector2 pos) { transform_.translate.x = pos.x, transform_.translate.y = pos.y; }
	void SetScale(const Vector2 scale) { transform_.scale.x = scale.x, transform_.scale.y = scale.y, transform_.scale.z = 1.0f; }
	/// <summary>
	/// 回転量の設定
	/// </summary>
	/// <param name="rotate">: ラジアン</param>
	void SetRotate(float rotate) { transform_.rotate.z = rotate; }
		 
private:

	// 定数バッファ
	ComPtr<ID3D12Resource> vertexBuffer_;
	ComPtr<ID3D12Resource> indexBuffer_;
	ComPtr<ID3D12Resource> materialBuffer_;
	ComPtr<ID3D12Resource> transformBuffer_;

	// view
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};
	D3D12_INDEX_BUFFER_VIEW indexBufferView_ = {};

	// データ
	TextureMesh* vertexData_;
	uint32_t* indexData_;
	TextureMaterial* materialData_;
	TextureTransformData* transformData_;

	Vector2 anchorPoint_;

	// Transform情報
	kTransform transform_;
	kTransform uvTransform_;

	std::string textureName_;

	// 描画する範囲
	Vector2 rectRange_ = { 0.0f, 0.0f };
	// 左上座標
	Vector2 leftTop_ = { 0.0f, 0.0f };
	Vector2 centerPos_; 

	// Textureのサイズ
	Vector2 textureSize_;
};