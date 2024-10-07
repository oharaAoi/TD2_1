#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Engine.h"
#include "Engine/Manager/ModelManager.h"
#include "Engine/GameObject/Model.h"
#include "Engine/Assets/WorldTransform.h"
#include "Engine/Assets/Animetor.h"
#include "Engine/Math/MyMatrix.h"
#include "Engine/Collider/Collider.h"
#include "Engine/Lib/GameTimer.h"

class BaseGameObject {
public:

	BaseGameObject() = default;
	virtual ~BaseGameObject() = default;

	virtual void Finalize();
	virtual void Init();
	virtual void Update();
	virtual void Draw() const;

#ifdef _DEBUG
	void Debug_Gui();
#endif // _DEBUG

	void SetObject(const std::string& objName);

	void SetAnimater(const std::string& directoryPath, const std::string& objName);

	WorldTransform* GetTransform() { return transform_.get(); }

	void SetColor(const Vector4& color);

protected:

	Model* model_;
	std::unordered_map<std::string, std::unique_ptr<Material>> materialArray_;	// 後で変えたい

	std::unique_ptr<WorldTransform> transform_;
	std::unique_ptr<Animetor> animetor_ = nullptr;

	bool isAnimation_ = false;

	Vector4 color_ = {1.0f, 1.0f, 1.0f, 1.0f};
};